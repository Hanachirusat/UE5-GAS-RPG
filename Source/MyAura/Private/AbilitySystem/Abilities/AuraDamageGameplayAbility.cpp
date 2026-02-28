


#include "AbilitySystem/Abilities/AuraDamageGameplayAbility.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "AuraGameplayTags.h"

void UAuraDamageGameplayAbility::CauseDamage(AActor* TargetActor)
{
	FGameplayEffectSpecHandle DamageSpecHandle = MakeOutgoingGameplayEffectSpec(DamageEffectClass, 1);
	// for (TTuple<FGameplayTag,FScalableFloat> Pair : DamageTypes)
	// {
	// 	float Damage = Pair.Value.GetValueAtLevel(GetAbilityLevel());
	// 	UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(DamageSpecHandle,Pair.Key,Damage);
	// }
	const float ScaledDamage = Damage.GetValueAtLevel(GetAbilityLevel());
	UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(DamageSpecHandle,DamageType,ScaledDamage);
	
	GetAbilitySystemComponentFromActorInfo()->ApplyGameplayEffectSpecToTarget(*DamageSpecHandle.Data.Get(),	UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(TargetActor));
	
}

FDamageEffectParams UAuraDamageGameplayAbility::MakeDamageEffectParamsFromClassDefaults(
		AActor* TargetActor,
		bool bOverrideKnockBackDirection,
		FVector InKnockbackDirectionOverride ,
		bool bOverrideDeathImpulse ,
		FVector DeathImpulseDirectionOverride,
		bool bOverridePitch ,
		float PitchOverride) const
{
	FDamageEffectParams Params;
	Params.WorldContextObject = GetAvatarActorFromActorInfo();
	Params.DamageGameplayEffectClass = DamageEffectClass;
	Params.SourceASC = GetAbilitySystemComponentFromActorInfo();
	Params.TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(TargetActor);
	Params.BaseDamage = Damage.GetValueAtLevel(GetAbilityLevel());
	Params.AbilityLevel = GetAbilityLevel();
	Params.DamageType = DamageType;
	Params.DebuffChance = DebuffChance;
	Params.DebuffDamage = DebuffDamage;
	Params.DebuffDuration = DebuffDuration;
	Params.DebuffFrequency = DebuffFrequency;
	Params.DeathImpulseMagnitude = DeathImpulseMagnitude;
	Params.KnockBackForceMagnitude = KnockBackForceMagnitude;
	Params.KnockBackChance = KnockBackChance;
	
	if (IsValid(TargetActor))
	{
		FRotator Rotation = (TargetActor->GetActorLocation() - GetAvatarActorFromActorInfo()->GetActorLocation()).Rotation();
		if (bOverridePitch)
		{
			Rotation.Pitch = PitchOverride;
		}
		const FVector ToTarget = Rotation.Vector();
		if (!bOverrideDeathImpulse)
		{
			Params.DeathImpulse = ToTarget * DeathImpulseMagnitude;
		}
		if (!bOverrideKnockBackDirection)
		{
			Params.KnockBackForce = ToTarget * KnockBackForceMagnitude;
		}
	}
	
	if (bOverrideKnockBackDirection)
	{
		InKnockbackDirectionOverride.Normalize();
		Params.KnockBackForce = InKnockbackDirectionOverride * KnockBackForceMagnitude;
		if (bOverridePitch)
		{
			FRotator KnockBackRotation = InKnockbackDirectionOverride.Rotation();
			KnockBackRotation.Pitch = PitchOverride;
			Params.KnockBackForce = KnockBackRotation.Vector() * KnockBackForceMagnitude;
		}
	}
	
	if (bOverrideDeathImpulse)
	{
		DeathImpulseDirectionOverride.Normalize();
		Params.DeathImpulse = DeathImpulseDirectionOverride * DeathImpulseMagnitude;
		if (bOverridePitch)
		{
			FRotator DeathImpulseRotation = DeathImpulseDirectionOverride.Rotation();
			DeathImpulseRotation.Pitch = PitchOverride;
			Params.DeathImpulse = DeathImpulseRotation.Vector() * DeathImpulseMagnitude;
		}
		
	}
	
	if (bIsRadialDamage)
	{
		Params.bIsRadialDamage = bIsRadialDamage;
		Params.RadialDamageInnerRadius  = RadialDamageInnerRadius;
		Params.RadialDamageOrigin = RadialDamageOrigin;
		Params.RadialDamageOuterRadius = RadialDamageOuterRadius;
	}
	return Params;
}

float UAuraDamageGameplayAbility::GetDamageAtLevel() const
{
	return Damage.GetValueAtLevel(GetAbilityLevel());
}

void UAuraDamageGameplayAbility::SetRadialDamageOrigin(const FVector& Origin)
{
	RadialDamageOrigin = Origin;
}

FTaggedMontage UAuraDamageGameplayAbility::GetRandomTaggedMontage(const TArray<FTaggedMontage>& TaggedMontage) const 
{
	if (!TaggedMontage.IsEmpty())
	{
		const int32 SelectIndex = FMath::RandRange(0, TaggedMontage.Num() - 1);
		return TaggedMontage[SelectIndex];
	}
	return FTaggedMontage();
}

// float UAuraDamageGameplayAbility::GetDamageByDamageType(float InLevel, const FGameplayTag& DamageType)
// {
// 	checkf(DamageTypes.Contains(DamageType),TEXT("GA[%s]伤害类型[%s]不存在（无法造成该类型伤害）"),*GetNameSafe(this),*DamageType.ToString());
// 	return DamageTypes[DamageType].GetValueAtLevel(InLevel);
// }
