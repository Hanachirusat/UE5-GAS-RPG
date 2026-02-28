


#include "AbilitySystem/EC/EC_Damage.h"
#include "AbilitySystemComponent.h"
#include "AuraAbilityTypes.h"
#include "AuraGameplayTags.h"
#include "AbilitySystem/AuraAttributeSet.h"
#include "AbilitySystem/BlueFuncLibrary.h"
#include "AbilitySystem/Data/CharacterClassInfo.h"
#include "Interaction/CombatInterface.h"
#include "Kismet/GameplayStatics.h"

struct AuraDamageStatics
{
	//属性捕获定义宏
	DECLARE_ATTRIBUTE_CAPTUREDEF(Armor);
	DECLARE_ATTRIBUTE_CAPTUREDEF(ArmorPenetration);
	DECLARE_ATTRIBUTE_CAPTUREDEF(BlockChance)
	DECLARE_ATTRIBUTE_CAPTUREDEF(CriticalHitChance)
	DECLARE_ATTRIBUTE_CAPTUREDEF(CriticalHitResistance)
	DECLARE_ATTRIBUTE_CAPTUREDEF(CriticalHitDamage)
	
	DECLARE_ATTRIBUTE_CAPTUREDEF(FireResistance)
	DECLARE_ATTRIBUTE_CAPTUREDEF(LightningResistance)
	DECLARE_ATTRIBUTE_CAPTUREDEF(ArcaneResistance)
	DECLARE_ATTRIBUTE_CAPTUREDEF(PhysicalResistance)
	
	TMap<FGameplayTag,FGameplayEffectAttributeCaptureDefinition> TagsToCaptureDefs;
	AuraDamageStatics()
	{
		//在AS中捕获Target的属性Armor，禁用快照
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet,Armor,Target,false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet,BlockChance,Target,false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet,ArmorPenetration,Source,false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet,CriticalHitChance,Source,false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet,CriticalHitResistance,Source,false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet,CriticalHitDamage,Source,false);
		
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet,FireResistance,Target,false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet,LightningResistance,Target,false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet,ArcaneResistance,Target,false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet,PhysicalResistance,Target,false);
	}
};

static const AuraDamageStatics& GetDamageStatics()
{
	static AuraDamageStatics DamageStatics;
	return DamageStatics;
}

UEC_Damage::UEC_Damage()
{
	RelevantAttributesToCapture.Add(AuraDamageStatics().ArmorDef);
	RelevantAttributesToCapture.Add(AuraDamageStatics().BlockChanceDef);
	RelevantAttributesToCapture.Add(AuraDamageStatics().ArmorPenetrationDef);
	RelevantAttributesToCapture.Add(AuraDamageStatics().CriticalHitChanceDef);
	RelevantAttributesToCapture.Add(AuraDamageStatics().CriticalHitResistanceDef);
	RelevantAttributesToCapture.Add(AuraDamageStatics().CriticalHitDamageDef);
	
	RelevantAttributesToCapture.Add(AuraDamageStatics().FireResistanceDef);
	RelevantAttributesToCapture.Add(AuraDamageStatics().LightningResistanceDef);
	RelevantAttributesToCapture.Add(AuraDamageStatics().ArcaneResistanceDef);
	RelevantAttributesToCapture.Add(AuraDamageStatics().PhysicalResistanceDef);
}

void UEC_Damage::DetermainDebuff(const FGameplayEffectCustomExecutionParameters& ExecutionParams, const FGameplayEffectSpec& Spec, FAggregatorEvaluateParameters EvaluateParameters,const TMap<FGameplayTag,FGameplayEffectAttributeCaptureDefinition>& TagsToCaptureDefs) const
{
	const FAuraGameplayTags GameplayTags = FAuraGameplayTags::Get();
	for (TTuple<FGameplayTag,FGameplayTag> Pair : GameplayTags.DamageTypesToDebuffs)
	{
		const FGameplayTag& DamageType = Pair.Key;
		const FGameplayTag& DebuffType = Pair.Value;
		//如果我们对某种伤害类型设置了伤害，则开始判定该伤害类型的Debuff是否生效
		const float TypeDamage = Spec.GetSetByCallerMagnitude(DamageType,false,-1.f);
		if (TypeDamage > -0.5f)
		{
			//确定是否成功应用Debuff
			const float SourceDebuffChance = Spec.GetSetByCallerMagnitude(GameplayTags.Debuff_Chance,false,-1.f);
			
			float TargetDebuffResistance = 0.f;
			const FGameplayTag& ResistanceTag = GameplayTags.DamageTypesToResistances[DamageType];
			ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(TagsToCaptureDefs[ResistanceTag],EvaluateParameters,TargetDebuffResistance);
			TargetDebuffResistance = FMath::Max<float>(TargetDebuffResistance,0.f);
			const float EffectiveDebuffChance = SourceDebuffChance *(100-TargetDebuffResistance)/100.f;
			const bool bDebuff = FMath::RandRange(1,100)<EffectiveDebuffChance;
			
			//成功判定Debuff
			if (bDebuff)
			{
				FGameplayEffectContextHandle ContextHandle = Spec.GetContext();
				UBlueFuncLibrary::SetIsSuccessfulDebuff(ContextHandle,true);
				UBlueFuncLibrary::SetDamageType(ContextHandle,DamageType);
				
				const float DebuffDamage = Spec.GetSetByCallerMagnitude(GameplayTags.Debuff_Damage,false,-1.f);
				const float DebuffFrequency = Spec.GetSetByCallerMagnitude(GameplayTags.Debuff_Frequency,false,-1.f);
				const float DebuffDuration = Spec.GetSetByCallerMagnitude(GameplayTags.Debuff_Duration,false,-1.f);
				UBlueFuncLibrary::SetDebuffDamage(ContextHandle,DebuffDamage);
				UBlueFuncLibrary::SetDebuffDuration(ContextHandle,DebuffDuration);
				UBlueFuncLibrary::SetDebuffFrequency(ContextHandle,DebuffFrequency);
			}
		}
	}
}

void UEC_Damage::Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams,
                                        FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const
{
	TMap<FGameplayTag,FGameplayEffectAttributeCaptureDefinition> TagsToCaptureDefs;
	const FAuraGameplayTags& Tag = FAuraGameplayTags::Get();
	TagsToCaptureDefs.Add(Tag.Attribute_Secondary_Armor,AuraDamageStatics().ArmorDef);
	TagsToCaptureDefs.Add(Tag.Attribute_Secondary_BlockChance,AuraDamageStatics().BlockChanceDef);
	TagsToCaptureDefs.Add(Tag.Attribute_Secondary_ArmorPenetration,AuraDamageStatics().ArmorPenetrationDef);
	TagsToCaptureDefs.Add(Tag.Attribute_Secondary_CriticalHitChance,AuraDamageStatics().CriticalHitChanceDef);
	TagsToCaptureDefs.Add(Tag.Attribute_Secondary_CriticalHitResistance,AuraDamageStatics().CriticalHitResistanceDef);
	TagsToCaptureDefs.Add(Tag.Attribute_Secondary_CriticalHitDamage,AuraDamageStatics().CriticalHitDamageDef);
		
	TagsToCaptureDefs.Add(Tag.Attribute_Resistance_Fire,AuraDamageStatics().FireResistanceDef);
	TagsToCaptureDefs.Add(Tag.Attribute_Resistance_Lightning,AuraDamageStatics().LightningResistanceDef);
	TagsToCaptureDefs.Add(Tag.Attribute_Resistance_Arcane,AuraDamageStatics().ArcaneResistanceDef);
	TagsToCaptureDefs.Add(Tag.Attribute_Resistance_Physical,AuraDamageStatics().PhysicalResistanceDef);
	
	const UAbilitySystemComponent* SourceASC = ExecutionParams.GetSourceAbilitySystemComponent();
	const UAbilitySystemComponent* TargetASC = ExecutionParams.GetTargetAbilitySystemComponent();	
	
	AActor* SourceAvatar = SourceASC? SourceASC->GetAvatarActor(): nullptr;
	AActor* TargetAvatar = TargetASC? TargetASC->GetAvatarActor(): nullptr;
	
	int32 SourceLevel = 1;
	if (SourceAvatar->Implements<UCombatInterface>())
	{
		SourceLevel=ICombatInterface::Execute_GetLevel(SourceAvatar);
	}
	int32 TargetLevel = 1;
	if (TargetAvatar->Implements<UCombatInterface>())
	{
		TargetLevel=ICombatInterface::Execute_GetLevel(TargetAvatar);
	}
	
	const FGameplayEffectSpec& Spec = ExecutionParams.GetOwningSpec();
	FGameplayEffectContextHandle EffectContextHandle = Spec.GetContext();
	const FGameplayTagContainer* SourceTags = Spec.CapturedSourceTags.GetAggregatedTags();
	const FGameplayTagContainer* TargetTags = Spec.CapturedTargetTags.GetAggregatedTags();
	FAggregatorEvaluateParameters EvaluateParameters;
	EvaluateParameters.SourceTags = SourceTags;
	EvaluateParameters.TargetTags = TargetTags;
	
	//Debuff
	DetermainDebuff(ExecutionParams, Spec, EvaluateParameters,TagsToCaptureDefs);
	
	
	float Damage = 0.f;
	//获取伤害
	for (auto& Pair : FAuraGameplayTags::Get().DamageTypesToResistances)
	{	
		const FGameplayTag DamageType = Pair.Key;
		const FGameplayTag ResistanceType = Pair.Value;
		
		//获取伤害
		float DamageTypeValue = Spec.GetSetByCallerMagnitude(DamageType,false);
		
		if (DamageTypeValue == 0.f)
		{
			continue;
		}
		
		//获取抗性
		float Resistance = 0.f;
		checkf(TagsToCaptureDefs.Contains(ResistanceType),TEXT("Key Error:Not Tag named [%s] in TagsToCaptureDefs"),*ResistanceType.ToString());
		const FGameplayEffectAttributeCaptureDefinition CaptureDef = TagsToCaptureDefs[ResistanceType];
		ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(CaptureDef,EvaluateParameters,Resistance);
		Resistance = FMath::Clamp(Resistance,0.f,100.f);

		//应用抗性
		DamageTypeValue *=(100.f - Resistance)/100.f;
		
		//判断是否是经向衰减伤害，如果是则衰减伤害
		if (UBlueFuncLibrary::IsRadialdamage(EffectContextHandle))
		{
			//1. Override TakeDamage in BaseCharacter
			//2. Create Delegate OnDamageDelegate, Brodacast Damage Receiver in TakeDamage
			//3. Bind toOnDamageDelegate on the victim here.
			//4. Call ApplyRadialDamageWithFallOff to Case Damage (this will result in takedamage being called)

			if (ICombatInterface* CombatInterface = Cast<ICombatInterface>(TargetAvatar))
			{
				CombatInterface->GetOnTakeDamageSignature().AddLambda([&](float DamageAmount)
				{
					DamageTypeValue = DamageAmount;
				});
			}
			UGameplayStatics::ApplyRadialDamageWithFalloff(
				TargetAvatar,
				DamageTypeValue,
				0.f,
				UBlueFuncLibrary::GetRadialDamageOrigin(EffectContextHandle),
				UBlueFuncLibrary::GetRadialDamageInnerRadius(EffectContextHandle),
				UBlueFuncLibrary::GetRadialDamageOuterRadius(EffectContextHandle),
				1.f,
				UDamageType::StaticClass(),
				TArray<AActor*>(),
				SourceAvatar,
				nullptr);
		}
		
		//累加伤害
		Damage+=DamageTypeValue;
	}
	
	//Begin 格挡率，判断是否格挡
	float TargetBlockChange = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(GetDamageStatics().BlockChanceDef,EvaluateParameters,TargetBlockChange);
	TargetBlockChange = FMath::Max<float>(TargetBlockChange,0.f);
	const bool bBlocked = FMath::RandRange(1,100)<TargetBlockChange;
	Damage = bBlocked ? Damage/2.f : Damage;
	UBlueFuncLibrary::SetIsBlockedHit(EffectContextHandle ,bBlocked);
	//End格挡率，判断是否格挡

	//Begin护甲穿透忽略目标护甲百分比
	float TargetArmor = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(GetDamageStatics().ArmorDef,EvaluateParameters,TargetArmor);
	TargetArmor = FMath::Max<float>(TargetArmor,0.f);
	
	float SourceArmorPenetration = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(GetDamageStatics().ArmorPenetrationDef,EvaluateParameters,SourceArmorPenetration);
	SourceArmorPenetration = FMath::Max<float>(SourceArmorPenetration,0.f);
	
	const UCharacterClassInfo* GetCharacterClassInfo = UBlueFuncLibrary::GetCharacterClassInfo(SourceAvatar);
	const FRealCurve* ArmorPenetrationCurve =  GetCharacterClassInfo->DamageCalculationCoefficients->FindCurve(FName("ArmorPenetration"),FString());
	const float ArmorPenetrationCoefficient = ArmorPenetrationCurve->Eval(SourceLevel);
	const FRealCurve* EffectiveArmorCurve =  GetCharacterClassInfo->DamageCalculationCoefficients->FindCurve(FName("EffectArmor"),FString());
	const float EffectiveArmorCoefficient = EffectiveArmorCurve->Eval(TargetLevel);
	
	const float EffectiveArmor = TargetArmor *=(100-SourceArmorPenetration * ArmorPenetrationCoefficient )/100.f;
	Damage *= (100-EffectiveArmor * EffectiveArmorCoefficient)/100.f;
	//End护甲穿透忽略目标护甲百分比
	
	//Begin暴击率计算
	float SourceCriticalHitChange = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(GetDamageStatics().CriticalHitChanceDef,EvaluateParameters,SourceCriticalHitChange);
	SourceCriticalHitChange = FMath::Max<float>(SourceCriticalHitChange,0.f);
	
	float SourceCriticalHitDamage = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(GetDamageStatics().CriticalHitDamageDef,EvaluateParameters,SourceCriticalHitDamage);
	SourceCriticalHitDamage = FMath::Max<float>(SourceCriticalHitDamage,0.f);
	
	float TargetCriticalHitResistance = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(GetDamageStatics().CriticalHitResistanceDef,EvaluateParameters,TargetCriticalHitResistance);
	TargetCriticalHitResistance = FMath::Max<float>(TargetCriticalHitResistance,0.f);
	
	const float EffectiveCriticalHitChange = SourceCriticalHitChange - TargetCriticalHitResistance*0.15f;
	const bool bCriticalHit = FMath::RandRange(1,100)<EffectiveCriticalHitChange;
	
	Damage = bCriticalHit ? Damage * (2.f+(SourceCriticalHitDamage/100)) : Damage;
	UBlueFuncLibrary::SetIsCriticalHit(EffectContextHandle ,bCriticalHit);
	//End暴击率计算
	
	FGameplayModifierEvaluatedData EvaluatedData(UAuraAttributeSet::GetIncomingDamageAttribute(),EGameplayModOp::Additive,Damage);
	OutExecutionOutput.AddOutputModifier(EvaluatedData);

}
