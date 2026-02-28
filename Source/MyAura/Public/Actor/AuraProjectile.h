

#pragma once

#include "CoreMinimal.h"
#include "AuraAbilityTypes.h"

#include "../../../../../../Program Files/Epic Games/UE_5.6/Engine/Plugins/FX/Niagara/Source/Niagara/Classes/NiagaraSystem.h"
#include "GameFramework/Actor.h"
#include "AuraProjectile.generated.h"

class USphereComponent;
class UProjectileMovementComponent;


UCLASS()
class MYAURA_API AAuraProjectile : public AActor
{
	GENERATED_BODY()
	
public:	
	AAuraProjectile();
	
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UProjectileMovementComponent> ProjectileMovementComponent;
	
	UPROPERTY(BlueprintReadWrite, meta=(ExposeOnSpawn = "true"))
	FDamageEffectParams  DamageEffectParams;

	UPROPERTY()
	TObjectPtr<USceneComponent> HomingTargetSceneComponent;
protected:
	UPROPERTY(VisibleAnywhere,BlueprintReadOnly)
	TObjectPtr<USphereComponent> Sphere;
	
	virtual void BeginPlay() override;
	
	UFUNCTION(BlueprintCallable)
	virtual void OnHit();
	virtual void Destroyed() override;
	
	UFUNCTION()
	virtual  void OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OhterComp,int32 OtherBodyIndex, bool bFromSweep,const FHitResult& SweepResult);
	
	UPROPERTY()
	TSet<AActor*> OverlapActor;
	
	bool bHit=false;
	
	bool IsValidOverlap(AActor* OtherActor);
	
	UPROPERTY(Editanywhere)
	TObjectPtr<UAudioComponent> LoopingSoundComponent;
	
private:
	UPROPERTY(Editanywhere)
	float LifeSpan = 15.f;
	

	
	UPROPERTY(Editanywhere)
	TObjectPtr<UNiagaraSystem> ImpactEffect;
	
	UPROPERTY(Editanywhere)
	TObjectPtr<USoundBase> ImpactSound;
	
	UPROPERTY(Editanywhere)
	TObjectPtr<USoundBase> LoopingSound;
	

	

};
