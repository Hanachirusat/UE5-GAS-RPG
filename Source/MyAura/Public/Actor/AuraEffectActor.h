

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameplayEffectTypes.h"
#include "AuraEffectActor.generated.h"


class UAbilitySystemComponent;
class UGameplayEffect;

UENUM(BlueprintType)
enum class EEffectApplicationPolicy : uint8
{
	ApplyOnOverlap,
	ApplyOnEndOverlap,
	DoNotApply
};

UENUM(BlueprintType)
enum class EEffectRemovalPolicy : uint8
{
	RemoveOnEndOverlap,
	DoNotRemove
};

UCLASS()
class MYAURA_API AAuraEffectActor : public AActor
{
	GENERATED_BODY()
	
public:	
	AAuraEffectActor();
	virtual void Tick(float DeltaTime) override;
	
protected:
	virtual void BeginPlay() override;
	
	UFUNCTION( BlueprintCallable )
	void ApplyEffectToTarget(AActor* TargetActor, TSubclassOf<UGameplayEffect> GameplayEffectClass);
	
	// UFUNCTION( BlueprintCallable )
	// void RemoveEffectToTarget(AActor* TargetActor, TSubclassOf<UGameplayEffect> GameplayEffectClass);
	
	UFUNCTION( BlueprintCallable )
	void OnOverlap(AActor* TargetActor);
	
	UFUNCTION( BlueprintCallable )
	void OnEndOverlap(AActor* TargetActor);
	
	//蓝图调用该函数后开始运动
	UFUNCTION( BlueprintCallable )
	void StartSinMovement();
	
	UFUNCTION( BlueprintCallable )
	void StartRotation();
	
	
	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category="Applied Effect")
	bool bDestroyOnEffectApplication = false;
	
	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category="Applied Effect")
	bool bApplyEffectToEnemies = false;
	
	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category="Applied Effect")
	TSubclassOf<UGameplayEffect> InstantGameplayEffectClass;
	
	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category="Applied Effect")
	EEffectApplicationPolicy InstantEffectApplicationPolicy = EEffectApplicationPolicy::DoNotApply;
	
	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category="Applied Effect")
	TSubclassOf<UGameplayEffect> DurationGameplayEffectClass;
	
	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category="Applied Effect")
	EEffectApplicationPolicy DurationEffectApplicationPolicy = EEffectApplicationPolicy::DoNotApply;
	
	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category="Applied Effect")
	TSubclassOf<UGameplayEffect> InfiniteGameplayEffectClass;
	
	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category="Applied Effect")
	EEffectApplicationPolicy InfiniteEffectApplicationPolicy = EEffectApplicationPolicy::DoNotApply;
	
	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category="Applied Effect")
	EEffectRemovalPolicy InfiniteEffectRemovalPolicy = EEffectRemovalPolicy::RemoveOnEndOverlap;

	
	TMap<FActiveGameplayEffectHandle,UAbilitySystemComponent*> ActiveEffectHandleMap;
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Applied Effect")
	float ActorLevel=1.f;
	
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Applied Effect")
	bool bRotates = false;
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Applied Effect")
	bool bUpAndDownMovement = false;
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Applied Effect")
	float RotationRate = 45.f;
		
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Applied Effect")
	float SinAmplitude = 8.f;
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Applied Effect")
	float SinPeriod = 6.28318f;
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Applied Effect")
	FVector InitialLocation;
	
	UPROPERTY(BlueprintReadWrite)
	FVector CalculatedLocation;
	
	UPROPERTY(BlueprintReadWrite)
	FRotator CalculatedRotation;
	
private:

	float RunningTime = 0.f;
	
	void ItemMovement(float DeltaTime);
};
