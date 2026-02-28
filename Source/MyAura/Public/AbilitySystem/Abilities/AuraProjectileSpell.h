

#pragma once

#include "CoreMinimal.h"
#include "AuraDamageGameplayAbility.h"
#include "AuraProjectileSpell.generated.h"

class UGameplayEffect;
class AAuraProjectile;

UCLASS()
class MYAURA_API UAuraProjectileSpell : public UAuraDamageGameplayAbility
{
	GENERATED_BODY()

protected:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	
	UFUNCTION(BlueprintCallable,Category="Projectile")
	void SpawnProjectile(const FVector& TargetLocation, const FGameplayTag& SocketTag);
	 
	UPROPERTY(EditAnywhere,BlueprintReadOnly)
	TSubclassOf<AAuraProjectile> ProjectileClass;
	
	UPROPERTY(EditDefaultsOnly)
	int32 MaxNumProjectiles=5;
};

