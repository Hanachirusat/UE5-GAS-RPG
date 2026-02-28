

#pragma once

#include "CoreMinimal.h"
#include "AuraAbilityTypes.h"
#include "AbilitySystem/Abilities/AuraGameplayAbility.h"
#include "Interaction/CombatInterface.h"
#include "AuraDamageGameplayAbility.generated.h"

/**
 * 
 */
UCLASS()
class MYAURA_API UAuraDamageGameplayAbility : public UAuraGameplayAbility
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintCallable)
	void CauseDamage(AActor* TargetActor);
	
	UFUNCTION(BlueprintPure)
	FDamageEffectParams MakeDamageEffectParamsFromClassDefaults(
		AActor* TargetActor = nullptr,
		bool bOverrideKnockBackDirection = false,
		FVector InKnockbackDirectionOverride = FVector::ZeroVector,
		bool bOverrideDeathImpulse = false,
		FVector DeathImpulseDirectionOverride = FVector::ZeroVector,
		bool bOverridePitch = false,
		float PitchOverride = 0.f) const;
	
	UFUNCTION(BlueprintCallable)
	float GetDamageAtLevel() const;
	
	UFUNCTION(BlueprintCallable)
	void SetRadialDamageOrigin(const FVector& Origin);
protected:
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly)
	TSubclassOf<UGameplayEffect> DamageEffectClass;
	
	// UPROPERTY(EditDefaultsOnly,Category="Damage")
	// TMap<FGameplayTag,FScalableFloat> DamageTypes;
	
	UPROPERTY(EditDefaultsOnly,Category="Damage")
	FGameplayTag DamageType;
	
	UPROPERTY(EditDefaultsOnly,Category="Damage")
	FScalableFloat Damage;
	
	UPROPERTY(EditDefaultsOnly,Category="Damage")
	float DebuffChance = 20.f;;
	
	UPROPERTY(EditDefaultsOnly,Category="Damage")
	float DebuffDamage = 2.f;;
	
	UPROPERTY(EditDefaultsOnly,Category="Damage")
	float DebuffFrequency = 0.5f;;
	
	UPROPERTY(EditDefaultsOnly,Category="Damage")
	float DebuffDuration = 3.f;;
	
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly, Category="Damage")
	float DeathImpulseMagnitude = 1000.f;;
	
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly,Category="Damage")
	float KnockBackForceMagnitude = 1000.f;;
	
	UPROPERTY(EditDefaultsOnly,Category="Damage")
	float KnockBackChance = 0.f;;
	
	/*经向衰减伤害相关参数*/
	
	UPROPERTY(EditDefaultsOnly,Category="Damage")
	bool bIsRadialDamage = false;
	
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly,Category="Damage")
	float RadialDamageInnerRadius = 0.f;
	
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly,Category="Damage")
	float RadialDamageOuterRadius = 0.f;
	
	UPROPERTY(EditDefaultsOnly,Category="Damage")
	FVector RadialDamageOrigin = FVector::ZeroVector;
	
	
	UFUNCTION(BlueprintPure)
	FTaggedMontage GetRandomTaggedMontage(const TArray<FTaggedMontage>& TaggedMontage) const ;
	
	// float GetDamageByDamageType(float InLevel, const FGameplayTag& DamageType);
};
