

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "GameplayTagContainer.h"
#include "AuraPlayerController.generated.h"

class IHighLightInterface;
class AMagicCircle;
class UNiagaraSystem;
class UDamageTextComponent;
class UAuraInputConfig;
/**
 * 
 */
class UInputMappingContext;
class UInputAction;
struct FInputActionValue;
class IEnemyInterface;
class UAuraAbilitySystemComponent;
class USplineComponent;

enum class ETargetingStatus :uint8
{
	TargetingEnemy,
	TargetingNotEnemy,
	NotTargeting
};
UCLASS()
class MYAURA_API AAuraPlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:
	AAuraPlayerController();
	virtual void PlayerTick(float DeltaTime) override;
	
	UFUNCTION(Client,Reliable)
	void ShowDamageNumber(float Damage,ACharacter* Target,bool bBlockedHit, bool bCriticalHit);
	
	UFUNCTION(BlueprintCallable)
	void ShowMagicCircle(UMaterialInterface* DecalMaterial = nullptr);
	
	UFUNCTION(BlueprintCallable)
	void HideMagicCircle();
protected:
	virtual void BeginPlay() override;
	virtual void SetupInputComponent() override;
private:
	UPROPERTY(EditAnywhere,Category="Input")
	TObjectPtr<UInputMappingContext> AuraInput;
	
	UPROPERTY(EditAnywhere,Category="Input")
	TObjectPtr<UInputAction> MoveAction;
	void Move(const FInputActionValue& InputActionValue);
	
	UPROPERTY(EditAnywhere,Category="Input")
	TObjectPtr<UInputAction> ShiftAction;
	void ShiftPressed() {bShiftPressed = true;};
	void ShiftReleased() {bShiftPressed = false;};
	bool bShiftPressed = false;
	
	
	void CursorTrace();
	UPROPERTY()
	TObjectPtr<AActor> LastActor;
	UPROPERTY()
	TObjectPtr<AActor> ThisActor;
	FHitResult CursorHitResult;
	static void HighLightActor(AActor* Actor);
	static void UnHighLightActor(AActor* Actor);
	
	void AbilityInputTagPressed(FGameplayTag tag);
	void AbilityInputTagReleased(FGameplayTag tag);
	void AbilityInputTagHeld(FGameplayTag tag);
	
	UPROPERTY(EditDefaultsOnly,Category="Input")
	TObjectPtr<UAuraInputConfig> AuraInputConfig;
	
	UPROPERTY()
	TObjectPtr<UAuraAbilitySystemComponent> AuraAbilitySystemComponent;
	
	UAuraAbilitySystemComponent* GetASC();
	
	
	//Click to Move 
	FVector CachedDestination = FVector::ZeroVector;
	float FollowTime = 0.f;
	float ShortPressThreshold = 0.5f;
	bool bAutoRunning = false; 
	
	UPROPERTY(EditDefaultsOnly,Category="Click to Move")
	float AutoRunAcceptanceRadius = 50.f;
	
	UPROPERTY(VisibleAnywhere,Category="Click to Move")
	TObjectPtr<USplineComponent> Spline;
	
	UPROPERTY(EditDefaultsOnly,Category="Click to Move")
	TObjectPtr<UNiagaraSystem> ClickEffect;
	//Attack
	ETargetingStatus TargetingStatus = ETargetingStatus::NotTargeting;
	
	void AutoRun();
	
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UDamageTextComponent> DamageTextComponent;
	
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<AMagicCircle> MagicCircleClass;
	
	UPROPERTY()
	TObjectPtr<AMagicCircle> MagicCircle;
	
	void UpdateMagicCircleLocation();
};
