

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "AuraWidgetController.generated.h"

class UAbilityInfo;
class UAuraAttributeSet;
class UAuraAbilitySystemComponent;
class AAuraPlayerState;
class AAuraPlayerController;
class UAbilitySystemComponent;
class UAttributeSet;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPlayerStateChangedSIgnature, int32, NewValue);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FAbilityInfoSignature, const FAuraAbilityInfo&, Info);


USTRUCT(BlueprintType)
struct FWidgetControllerParams
{
	GENERATED_BODY()
	FWidgetControllerParams(){}
	FWidgetControllerParams(
		APlayerController* PC,
		APlayerState* PS,
		UAbilitySystemComponent* ASC,
		UAttributeSet* AS) : PlayerController(PC),PlayerState(PS),AbilitySystemComponent(ASC),AttributeSet(AS){}
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	TObjectPtr<APlayerController> PlayerController = nullptr;
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	TObjectPtr<APlayerState> PlayerState = nullptr;
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent = nullptr;
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	TObjectPtr<UAttributeSet> AttributeSet = nullptr;	
	
};
UCLASS()
class MYAURA_API UAuraWidgetController : public UObject
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintCallable)
	void SetWidgetControllerParams(const FWidgetControllerParams& WCParams);
	
	UFUNCTION(BluePrintCallable)
	virtual void BroadcastInitialValue();
	virtual void BindCallbacksToDependencies();
	
	void BroadcastAbilityInfo();
	UPROPERTY(BlueprintAssignable,Category="GAS|Message") 
	FAbilityInfoSignature AbilityInfoDelegate;
	
	
protected:
	
	UPROPERTY(BlueprintReadOnly,Category="AuraWidgetController")
	TObjectPtr<APlayerController> PlayerController;
	
	UPROPERTY(BlueprintReadOnly,Category="AuraWidgetController")
	TObjectPtr<APlayerState> PlayerState;
	
	UPROPERTY(BlueprintReadOnly,Category="AuraWidgetController")
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;
	
	UPROPERTY(BlueprintReadOnly,Category="AuraWidgetController")
	TObjectPtr<UAttributeSet> AttributeSet;	
	
	UPROPERTY(BlueprintReadOnly,Category="AuraWidgetController")
	TObjectPtr<AAuraPlayerController> AuraPlayerController;
	
	UPROPERTY(BlueprintReadOnly,Category="AuraWidgetController")
	TObjectPtr<AAuraPlayerState> AuraPlayerState;
	
	UPROPERTY(BlueprintReadOnly,Category="AuraWidgetController")
	TObjectPtr<UAuraAbilitySystemComponent> AuraAttributeSystemComponent;
	
	UPROPERTY(BlueprintReadOnly,Category="AuraWidgetController")
	TObjectPtr<UAuraAttributeSet> AuraAttributeSet;	
	
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly,Category = "widget Data")
	TObjectPtr<UAbilityInfo> AbilityInfo;;
	
	AAuraPlayerController* GetAuraPC();
	AAuraPlayerState* GetAuraPS();
	UAuraAttributeSet* GetAuraAS();
	UAuraAbilitySystemComponent* GetAuraASC();
};
