

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/AuraProjectileSpell.h"
#include "AuraFireBolt.generated.h"


UCLASS()
class MYAURA_API UAuraFireBolt : public UAuraProjectileSpell
{
	GENERATED_BODY()
public:
	virtual FString GetDescription(int32 Level) override;
	virtual FString GetNextLevelDescription(int32 Level) override;
	
	UFUNCTION(BlueprintCallable)
	void SpawnProjectiles(const FVector& ProjectileTargetLocation, const FGameplayTag& SocketTag,bool bOverridePitch, float PitchOverride, AActor* Homingtarget);
protected:
	
	UPROPERTY(EditDefaultsOnly,Category = "Firebolt")
	float ProjectileSpread = 90.f;
	
	UPROPERTY(EditDefaultsOnly,Category = "Firebolt")
	int32 NumProjectiles = 5.f;
	
	UPROPERTY(EditDefaultsOnly,Category = "Firebolt")
	float HomingAcceleration = 1600.f;
	
	UPROPERTY(EditDefaultsOnly,Category = "Firebolt")
	bool bIsHomingProjectile = true;
};
