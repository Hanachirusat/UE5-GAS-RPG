

#pragma once

#include "CoreMinimal.h"
#include "AuraProjectile.h"
#include "GameFramework/Actor.h"
#include "AuraFireBall.generated.h"

UCLASS()
class MYAURA_API AAuraFireBall : public AAuraProjectile
{
	GENERATED_BODY()
	
public:
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;
	UFUNCTION(BlueprintImplementableEvent)
	void StartOutgoingTimeline();
	
	UPROPERTY(BlueprintReadWrite)
	TObjectPtr<AActor> ReturnToActor;
	
	UPROPERTY(BlueprintReadWrite)
	FDamageEffectParams ExplosionDamageParams;
	
	UPROPERTY(Replicated,BlueprintReadWrite)
	bool Explosion;
	
protected:
	virtual void BeginPlay() override;
	virtual void OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OhterComp,int32 OtherBodyIndex, bool bFromSweep,const FHitResult& SweepResult) override;
	
	virtual void OnHit() override;


};
