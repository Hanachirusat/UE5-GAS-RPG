

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Kismet/BlueprintAsyncActionBase.h"
#include "WaitCooldownChange.generated.h"

class UAbilitySystemComponent;
struct FActiveGameplayEffectHandle;
struct FGameplayEffectSpec;
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FCooldownChangeSignature, float, TimeRemaining);

UCLASS(BlueprintType,meta=(ExposedAsyncProxy= "AsyncTask"))
class MYAURA_API UWaitCooldownChange : public UBlueprintAsyncActionBase
{
	GENERATED_BODY()
	
public:
	UPROPERTY(BlueprintAssignable)
	FCooldownChangeSignature CooldownStart;
	
	UPROPERTY(BlueprintAssignable)
	FCooldownChangeSignature CooldownEnd;
	
	UFUNCTION(BlueprintCallable,meta = (BlueprintInternalUseOnly = "true"))
	static UWaitCooldownChange* CreateCooldownChange(UAbilitySystemComponent* InASC, const FGameplayTag& InCooldownTag);
	
	UFUNCTION(BlueprintCallable)
	void EndTask();
	
protected:
	UPROPERTY()
	TObjectPtr<UAbilitySystemComponent> ASC;
	
	UPROPERTY()
	FGameplayTag CooldownTag;
	
	void CooldownTagChanged(const FGameplayTag InCooldownTag, int32 NewCount);
	
	void OnActivateEffectAdded(UAbilitySystemComponent* TargetASC,const FGameplayEffectSpec& SpecApplied,FActiveGameplayEffectHandle ActiveEffectHandle);
};
