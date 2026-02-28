


#include "AbilitySystem/AuraAttributeSet.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "Net/UnrealNetwork.h"
#include "AbilitySystemComponent.h"
#include "AuraAbilityTypes.h"
#include "GameplayEffectExtension.h"
#include "GameplayEffectTypes.h"
#include "GameFramework/Character.h"
#include "AuraGameplayTags.h"
#include "AbilitySystem/BlueFuncLibrary.h"
#include "GameplayEffectComponents/TargetTagsGameplayEffectComponent.h"
#include "Interaction/CombatInterface.h"
#include "Interaction/PlayerInterface.h"
#include "Player/AuraPlayerController.h"

UAuraAttributeSet::UAuraAttributeSet()
{
	const FAuraGameplayTags& GameplayTags = FAuraGameplayTags::Get();
	
	//Primary Attribute
	TagsToAttributes.Add(GameplayTags.Attribute_Primary_Strength,GetStrengthAttribute);
	TagsToAttributes.Add(GameplayTags.Attribute_Primary_Intelligence,GetIntelligenceAttribute);
	TagsToAttributes.Add(GameplayTags.Attribute_Primary_Resilience,GetResilienceAttribute);
	TagsToAttributes.Add(GameplayTags.Attribute_Primary_Vigor,GetVigorAttribute);
	
	//Secondary Attribute
	TagsToAttributes.Add(GameplayTags.Attribute_Secondary_Armor,GetArmorAttribute);
	TagsToAttributes.Add(GameplayTags.Attribute_Secondary_ArmorPenetration,GetArmorPenetrationAttribute);
	TagsToAttributes.Add(GameplayTags.Attribute_Secondary_BlockChance,GetBlockChanceAttribute);
	TagsToAttributes.Add(GameplayTags.Attribute_Secondary_CriticalHitChance,GetCriticalHitChanceAttribute);
	TagsToAttributes.Add(GameplayTags.Attribute_Secondary_CriticalHitDamage,GetCriticalHitDamageAttribute);
	TagsToAttributes.Add(GameplayTags.Attribute_Secondary_CriticalHitResistance,GetCriticalHitResistanceAttribute);
	TagsToAttributes.Add(GameplayTags.Attribute_Secondary_HealthRegeneration,GetHealthRegenerationAttribute);
	TagsToAttributes.Add(GameplayTags.Attribute_Secondary_ManaRegeneration,GetManaRegenerationAttribute);
	TagsToAttributes.Add(GameplayTags.Attribute_Secondary_MaxHP,GetMaxHPAttribute);
	TagsToAttributes.Add(GameplayTags.Attribute_Secondary_MaxMana,GetMaxManaAttribute);
	
	TagsToAttributes.Add(GameplayTags.Attribute_Resistance_Fire,GetFireResistanceAttribute);
	TagsToAttributes.Add(GameplayTags.Attribute_Resistance_Lightning,GetLightningResistanceAttribute);
	TagsToAttributes.Add(GameplayTags.Attribute_Resistance_Arcane,GetArcaneResistanceAttribute);
	TagsToAttributes.Add(GameplayTags.Attribute_Resistance_Physical,GetPhysicalResistanceAttribute);
	
	
}

//复制函数
void UAuraAttributeSet::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	
	//注册HP变量的复制，无条件复制，只要服务端发生了变化就进行复制
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet,HP,COND_None,REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet,MaxHP,COND_None,REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet,Mana,COND_None,REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet,MaxMana,COND_None,REPNOTIFY_Always);
	
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet,Strength,COND_None,REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet,Intelligence,COND_None,REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet,Resilience,COND_None,REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet,Vigor,COND_None,REPNOTIFY_Always);
	
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, Armor, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, ArmorPenetration, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, BlockChance, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, CriticalHitChance, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, CriticalHitDamage, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, CriticalHitResistance, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, HealthRegeneration, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, ManaRegeneration, COND_None, REPNOTIFY_Always);
	
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, FireResistance, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, LightningResistance, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, ArcaneResistance, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, PhysicalResistance, COND_None, REPNOTIFY_Always);
}

void UAuraAttributeSet::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{
	Super::PreAttributeChange(Attribute, NewValue);
	
	
	if (Attribute == GetHPAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.f, GetMaxHP());
	}
	if (Attribute == GetManaAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.f, GetMaxMana());
	}
}

void UAuraAttributeSet::HandleIncomingDamage(const FEffectProperties& EffectProperties)
{
	const float LocalIncomingDamage = GetIncomingDamage();
	SetIncomingDamage(0.f);
	if (LocalIncomingDamage>0.f)
	{
		const float NewHP = GetHP()-LocalIncomingDamage;
		SetHP(FMath::Clamp(NewHP,0.f,GetMaxHP()));
			
		const bool bFatal = NewHP<=0.f;
		if (bFatal)
		{
			ICombatInterface* CombatInterface = Cast<ICombatInterface>(EffectProperties.TargetAvatarActor);
			if (CombatInterface)
			{
				CombatInterface->Die(UBlueFuncLibrary::GetDeathImpulse(EffectProperties.EffectContextHandle));
			}
			SendXPEvent(EffectProperties);
		}
		else
		{
			//激活敌方的受击技能
			if (EffectProperties.TargetCharacter->Implements<UCombatInterface>() && !ICombatInterface::Execute_GetIsBeingShock(EffectProperties.TargetCharacter))
			{
				FGameplayTagContainer TagContainer;
				TagContainer.AddTag(FAuraGameplayTags::Get().Abilities_HitReact);
				EffectProperties.TargetASC->TryActivateAbilitiesByTag(TagContainer);
			}
			
			//在Actor中判断是否击飞，也可以在这里判断
			const FVector& KnockBackForce = UBlueFuncLibrary::GetKnockBackForce(EffectProperties.EffectContextHandle);
			if (!KnockBackForce.IsNearlyZero())
			{
				//角色只在xy轴被推动
				EffectProperties.TargetCharacter->LaunchCharacter(KnockBackForce,true,true);
			}
		}
		
		//判断是否暴击或者格挡？（不同的UI）
		const bool bBlocked = UBlueFuncLibrary::IsBlockedHit(EffectProperties.EffectContextHandle);
		const bool bCritical = UBlueFuncLibrary::IsCriticalHit(EffectProperties.EffectContextHandle);
		ShowFloatingText(EffectProperties,LocalIncomingDamage,bBlocked,bCritical);
			
		if (UBlueFuncLibrary::IsSuccessfulDebuff(EffectProperties.EffectContextHandle))
		{
			//处理Debuff
			Debuff(EffectProperties);
		}
	}
}

void UAuraAttributeSet::HandleIncomingXP(const FEffectProperties& EffectProperties)
{
	const float LocalIncomingXP = GetIncomingXP();
	SetIncomingXP(0.f);
	//Source Character is the Owner
	if (EffectProperties.SourceCharacter->Implements<UPlayerInterface>() && EffectProperties.SourceCharacter->Implements<UCombatInterface>())
	{
		const int32 CurrentLevel = ICombatInterface::Execute_GetLevel(EffectProperties.SourceCharacter);
		const int32 CurrentXP = IPlayerInterface::Execute_GetXP(EffectProperties.SourceCharacter);
			
		const int32 NewLevel = IPlayerInterface::Execute_FindLevelForXP(EffectProperties.SourceCharacter,CurrentXP+LocalIncomingXP);
		const int32 NumLevelUps = NewLevel-CurrentLevel;
		if (NumLevelUps>0)
		{
			IPlayerInterface::Execute_AddToLevel(EffectProperties.SourceCharacter,NumLevelUps);
			
			int32 AttributeReward = 0;
			int32 SpellReward = 0;
			
			for (int32 i = 0;i<NumLevelUps;i++)
			{
				AttributeReward+=IPlayerInterface::Execute_GetAttributePointReward(EffectProperties.SourceCharacter,CurrentLevel+i);
				SpellReward += IPlayerInterface::Execute_GetSpellPointReward(EffectProperties.SourceCharacter,CurrentLevel+i);
			
			}
			IPlayerInterface::Execute_AddToAttributePoints(EffectProperties.SourceCharacter,AttributeReward);
			IPlayerInterface::Execute_AddToSepllPoints(EffectProperties.SourceCharacter,SpellReward);
				
			bTopOffHealth  = true;
			bTopOffMana    = true;
				
			//升级效果
			IPlayerInterface::Execute_LevelUp(EffectProperties.SourceCharacter);
		}
		IPlayerInterface::Execute_AddToXP(EffectProperties.SourceCharacter,LocalIncomingXP);
	}
}

//Dynamic GameplayEffect 没有被复制
void UAuraAttributeSet::Debuff(const FEffectProperties& EffectProperties)
{
	const FAuraGameplayTags GameplayTags = FAuraGameplayTags::Get();
	FGameplayEffectContextHandle EffectContext = EffectProperties.SourceASC->MakeEffectContext();
	EffectContext.AddSourceObject(EffectProperties.SourceCharacter);
	
	const FGameplayTag DamageType = UBlueFuncLibrary::GetDamageType(EffectProperties.EffectContextHandle);
	const float DebuffDamage = UBlueFuncLibrary::GetDebuffDamage(EffectProperties.EffectContextHandle);
	const float DebuffDuration = UBlueFuncLibrary::GetDebuffDuration(EffectProperties.EffectContextHandle);
	const float DebuffFrequency = UBlueFuncLibrary::GetDebuffFrequency(EffectProperties.EffectContextHandle);
	
	FString DebuffName = FString::Printf(TEXT("DynamicDebuff_%s"),*DamageType.ToString());
	UGameplayEffect* Effect  = NewObject<UGameplayEffect>(GetTransientPackage(),FName(DebuffName));
	
	Effect->DurationPolicy = EGameplayEffectDurationType::HasDuration;
	Effect->Period = DebuffFrequency;
	Effect->DurationMagnitude = FScalableFloat(DebuffDuration);
	
	const FGameplayTag DebuffTag = GameplayTags.DamageTypesToDebuffs[DamageType];
	UTargetTagsGameplayEffectComponent& EffectComponent = Effect->AddComponent<UTargetTagsGameplayEffectComponent>();
	FInheritedTagContainer InheritedTagContainer;
	InheritedTagContainer.AddTag(DebuffTag);

	//如果被麻痹，则不能进行这些操作,但这只发生在服务端。
	if (DebuffTag.MatchesTagExact(GameplayTags.Debuff_Lightning))
	{
		InheritedTagContainer.AddTag(GameplayTags.Player_Block_CursorTrace);
		InheritedTagContainer.AddTag(GameplayTags.Player_Block_InputHeld);
		InheritedTagContainer.AddTag(GameplayTags.Player_Block_InputPressed);
		InheritedTagContainer.AddTag(GameplayTags.Player_Block_InputReleased);
	}
	EffectComponent.SetAndApplyTargetTagChanges(InheritedTagContainer);

	
	Effect->StackingType = EGameplayEffectStackingType::AggregateBySource;
	Effect->StackLimitCount = 1;
	
	//增加一个Modifier
	const int32 Idx = Effect->Modifiers.Num();
	Effect->Modifiers.Add(FGameplayModifierInfo());
	FGameplayModifierInfo& ModifierInfo = Effect->Modifiers[Idx];
	
	ModifierInfo.ModifierMagnitude = FScalableFloat(DebuffDamage);
	ModifierInfo.ModifierOp = EGameplayModOp::AddFinal;
	ModifierInfo.Attribute = UAuraAttributeSet::GetIncomingDamageAttribute();
	
	//可以深入Debuff的等级
	if (FGameplayEffectSpec* MutableSpec = new FGameplayEffectSpec(Effect,EffectContext,1.f))
	{
		FAuraGameplayEffectContext* AuraContext = static_cast<FAuraGameplayEffectContext*>(EffectContext.Get());
		//这里可以创建一个局部变量TSharedPtr
		AuraContext->SetDamageType(MakeShared<FGameplayTag>(DamageType));
		EffectProperties.TargetASC->ApplyGameplayEffectSpecToSelf(*MutableSpec);
	}
}

void UAuraAttributeSet::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);
	
	FEffectProperties EffectProperties;
	SetEffectProperties(Data,EffectProperties);  //Get info of Source and target from data

	if (EffectProperties.TargetCharacter->Implements<UCombatInterface>())
	{
		if (ICombatInterface::Execute_IsDead(EffectProperties.TargetCharacter))
		{
			return;
		}
	}
	if (Data.EvaluatedData.Attribute == GetHPAttribute())
	{
		SetHP(FMath::Clamp(GetHP(),0.f,GetMaxHP()));
	}
	if (Data.EvaluatedData.Attribute == GetManaAttribute())
	{
		SetMana(FMath::Clamp(GetMana(),0.f,GetMaxMana()));
	}
	if (Data.EvaluatedData.Attribute == GetIncomingDamageAttribute())  //GE修改的是MetaAttribute
	{
		HandleIncomingDamage(EffectProperties);
	}
	if (Data.EvaluatedData.Attribute == GetIncomingXPAttribute())
	{
		HandleIncomingXP(EffectProperties);
	}
}

void UAuraAttributeSet::PostAttributeChange(const FGameplayAttribute& Attribute, float OldValue, float NewValue)
{
	Super::PostAttributeChange(Attribute, OldValue, NewValue);
	
	if (Attribute == GetMaxHPAttribute() && bTopOffHealth)
	{
		SetHP(GetMaxHP());
		bTopOffHealth  = false;
	}
	if (Attribute == GetMaxManaAttribute() && bTopOffMana)
	{
		SetMana(GetMaxMana());
		bTopOffMana  = false;
	}
}

void UAuraAttributeSet::SendXPEvent(const FEffectProperties& Props)
{
	if (Props.TargetCharacter->Implements<UCombatInterface>())
	{
		const int32 TargetLevel = ICombatInterface::Execute_GetLevel(Props.TargetCharacter);
		const ECharacterClass TargetClass = ICombatInterface::Execute_GetCharacterClass(Props.TargetCharacter);
		const int32 XPReward = UBlueFuncLibrary::GetXpRewardForClassAndLevel(Props.TargetCharacter,TargetClass,TargetLevel);
		
		const FGameplayTag GameplayTag = FAuraGameplayTags::Get().Attribute_Meta_IncomingXP;
		FGameplayEventData EventData;
		EventData.EventTag = GameplayTag;
		EventData.EventMagnitude = XPReward;
		UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(Props.SourceCharacter,GameplayTag,EventData);
	}
}

void UAuraAttributeSet::ShowFloatingText(const FEffectProperties& EffectProperties, float Damage, bool bBlockedHit, bool bCriticalHit) const
{
	// // 打印布尔参数
	// UE_LOG(LogTemp, Warning, TEXT("ShowFloatingText - bBlockedHit: %s, bCriticalHit: %s"), 
	// 	bBlockedHit ? TEXT("true") : TEXT("false"), 
	// 	bCriticalHit ? TEXT("true") : TEXT("false"));
	
	if (EffectProperties.SourceCharacter!=EffectProperties.TargetCharacter)
	{
		//0这里获得的是服务端的控制器。，我们需要获取的原角色的控制器
		if (AAuraPlayerController* PC = Cast<AAuraPlayerController>(EffectProperties.SourceCharacter->Controller))
		{
			PC->ShowDamageNumber(Damage,EffectProperties.TargetCharacter, bBlockedHit, bCriticalHit);
			return ;
		}
		//敌人如果想要显示伤害数字，需要获取到目标的控制器，也就是玩家的控制器，然后在TargetCharacter处显示伤害数字，
		if (AAuraPlayerController* PC = Cast<AAuraPlayerController>(EffectProperties.TargetCharacter->Controller))
		{
			PC->ShowDamageNumber(Damage,EffectProperties.TargetCharacter, bBlockedHit, bCriticalHit);
		}
	}
}



void UAuraAttributeSet::OnRep_HP(const FGameplayAttributeData& OldHp) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, HP, OldHp);   //告诉GAS我们在同步这个属性，并记录旧值以便回滚。
}
void UAuraAttributeSet::OnRep_MaxHP(const FGameplayAttributeData& OldMaxHP) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, MaxHP, OldMaxHP);   //告诉GAS我们在同步这个属性，并记录旧值以便回滚。
}

void UAuraAttributeSet::OnRep_Mana(const FGameplayAttributeData& OldMana) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, Mana, OldMana);   //告诉GAS我们在同步这个属性，并记录旧值以便回滚。
}

void UAuraAttributeSet::OnRep_MaxMana(const FGameplayAttributeData& OldMaxMana) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, MaxMana, OldMaxMana);   //告诉GAS我们在同步这个属性，并记录旧值以便回滚。
}

void UAuraAttributeSet::OnRep_Strength(const FGameplayAttributeData& OldStrength) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, Strength, OldStrength);
}

void UAuraAttributeSet::OnRep_Intelligence(const FGameplayAttributeData& OldIntelligence) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, Intelligence, OldIntelligence);
}

void UAuraAttributeSet::OnRep_Resilience(const FGameplayAttributeData& OldResilience) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, Resilience, OldResilience);
}

void UAuraAttributeSet::OnRep_Viger(const FGameplayAttributeData& OldViger) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, Vigor, OldViger);
}

void UAuraAttributeSet::OnRep_Armor(const FGameplayAttributeData& OldArmor) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, Armor, OldArmor);
}

void UAuraAttributeSet::OnRep_ArmorPenetration(const FGameplayAttributeData& OldArmorPenetration) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, ArmorPenetration, OldArmorPenetration);
}

void UAuraAttributeSet::OnRep_BlockChance(const FGameplayAttributeData& OldBlockChance) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, BlockChance, OldBlockChance);
}

void UAuraAttributeSet::OnRep_CriticalHitChance(const FGameplayAttributeData& OldCriticalHitChance) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, CriticalHitChance, OldCriticalHitChance);
}

void UAuraAttributeSet::OnRep_CriticalHitDamage(const FGameplayAttributeData& OldCriticalHitDamage) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, CriticalHitDamage, OldCriticalHitDamage);
}

void UAuraAttributeSet::OnRep_CriticalHitResistance(const FGameplayAttributeData& OldCriticalHitResistance) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, CriticalHitResistance, OldCriticalHitResistance);
}

void UAuraAttributeSet::OnRep_HealthRegeneration(const FGameplayAttributeData& OldHealthRegeneration) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, HealthRegeneration, OldHealthRegeneration);
}

void UAuraAttributeSet::OnRep_ManaRegeneration(const FGameplayAttributeData& OldManaRegeneration) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, ManaRegeneration, OldManaRegeneration);
}

void UAuraAttributeSet::OnRep_FireResistance(const FGameplayAttributeData& OldFireResistance) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, FireResistance, OldFireResistance);
}

void UAuraAttributeSet::OnRep_LightningResistance(const FGameplayAttributeData& OldLightningResistance) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, LightningResistance, OldLightningResistance);
}

void UAuraAttributeSet::OnRep_ArcaneResistance(const FGameplayAttributeData& OldArcaneResistance) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, ArcaneResistance, OldArcaneResistance);
}

void UAuraAttributeSet::OnRep_PhysicalResistance(const FGameplayAttributeData& OldPhysicalResistance) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, PhysicalResistance, OldPhysicalResistance);
}

void UAuraAttributeSet::SetEffectProperties(const FGameplayEffectModCallbackData& Data,
                                            FEffectProperties& EffectProperties) const
{
	//Source = causer of the effect ,target = target of the effect (owner of this As)
	
	EffectProperties.EffectContextHandle = Data.EffectSpec.GetContext();
	EffectProperties.SourceASC  = EffectProperties.EffectContextHandle.GetOriginalInstigatorAbilitySystemComponent();
	
	if (IsValid(EffectProperties.SourceASC) && EffectProperties.SourceASC->AbilityActorInfo.IsValid() && EffectProperties.SourceASC->AbilityActorInfo->AvatarActor.IsValid())
	{
		//try to get controller from ASC
		EffectProperties.SourceController = EffectProperties.SourceASC->AbilityActorInfo->PlayerController.Get();
		EffectProperties.SourceAvatarActor = EffectProperties.SourceASC->AbilityActorInfo->AvatarActor.Get();
		//try to get controller from Acatar
		if (EffectProperties.SourceController == nullptr && EffectProperties.SourceAvatarActor != nullptr)
		{
			if (APawn* Pawn = Cast<APawn>(EffectProperties.SourceAvatarActor))
			{
				EffectProperties.SourceController = Pawn->GetController();
			}
		}
		//try to get Character from controller
		if (EffectProperties.SourceController)
		{
			EffectProperties.SourceCharacter = Cast<ACharacter>(EffectProperties.SourceController->GetPawn());
		}
	}
	if (Data.Target.AbilityActorInfo.IsValid() && Data.Target.AbilityActorInfo->AvatarActor.IsValid())
	{
		EffectProperties.TargetAvatarActor = Data.Target.AbilityActorInfo->AvatarActor.Get();
		EffectProperties.TargetController = Data.Target.AbilityActorInfo->PlayerController.Get();
		EffectProperties.TargetCharacter = Cast<ACharacter>(EffectProperties.TargetAvatarActor);
		EffectProperties.TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(EffectProperties.TargetAvatarActor);
	}
}

