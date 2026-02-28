

#pragma once

#include "CoreMinimal.h"
#include "AuraGameplayTags.h"
#include "GameplayTagContainer.h"
#include "UI/WidgetController/AuraWidgetController.h"
#include "SpellMenuWidgetController.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FSpellGlobeSelectedSignature, bool,bSpend,bool,bEquip,FString, LevelDescription,FString,NextLevelDescription);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FWaiteForEquipSelectionSignature, const FGameplayTag&,AbilityTag);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FSpellGlobeReassignedSignature, const FGameplayTag&,AbilityTag);

struct FSelectedAbility
{
	FGameplayTag AbilityTag = FGameplayTag();
	FGameplayTag StatusTag = FGameplayTag();
	FGameplayTag SelectedSlot = FGameplayTag();
};

UCLASS(BlueprintType,Blueprintable)
class MYAURA_API USpellMenuWidgetController : public UAuraWidgetController
{
	GENERATED_BODY()
	
public:
	UPROPERTY(BlueprintAssignable)
	FSpellGlobeSelectedSignature SpellGlobeSelectedDelegate;
	
	UPROPERTY(BlueprintAssignable)
	FOnPlayerStateChangedSIgnature SpellPointChanged; 
	
	UPROPERTY(BlueprintAssignable)
	FWaiteForEquipSelectionSignature WaiteForEquipSelectionDelegate;
	
	UPROPERTY(BlueprintAssignable)
	FWaiteForEquipSelectionSignature StopWaiteForEquipSelectionDelegate;
	
	UPROPERTY(BlueprintAssignable)
	FSpellGlobeReassignedSignature SpellGlobeReassignedDelegate;
	
	virtual void BroadcastInitialValue() override;
	virtual void BindCallbacksToDependencies() override;
	
	UFUNCTION(BlueprintCallable)
	void SpellGlobeSelected(const FGameplayTag& AbilityTag);
	
	UFUNCTION(BlueprintCallable)
	void SpendPointButtonPressed();
	
	UFUNCTION(BlueprintCallable)
	void GlobeDeselect();
	
	UFUNCTION(BlueprintCallable)
	void EquipButtonPressed();
	
	UFUNCTION(BlueprintCallable)
	void SpellRowGlobePressed(const FGameplayTag& SlotTag, const FGameplayTag& AbilityType);
	
	void OnAbilityEquipped(const FGameplayTag& AbilityTag,const FGameplayTag& Status, const FGameplayTag& Slot,const FGameplayTag& PrevSlot);
	
private:
	static void ShouldEnableButtons(const FGameplayTag& AbilityStatus,int32 SpellPoints,bool& bSpend,bool& bEquip);
	FSelectedAbility SelectAbility = {FAuraGameplayTags::Get().Abilities_None,FAuraGameplayTags::Get().Abilities_Status_Locked};
	int32 CurrentSpellPints = 0;
	bool bWaiteForEquipSelection = false;
	// FGameplayTag SelectedSlot;
};

