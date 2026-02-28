

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "AuraAbilitySystemComponent.generated.h"


class ULoadScreenSaveGame;
class UAuraAbilitySystemComponent;
DECLARE_MULTICAST_DELEGATE_OneParam(FEffectAssetTags, const FGameplayTagContainer& /*AssetTags*/);
DECLARE_MULTICAST_DELEGATE(FAbilityGiven);
DECLARE_DELEGATE_OneParam(FForEachAbility, const FGameplayAbilitySpec&);
DECLARE_MULTICAST_DELEGATE_ThreeParams(FAbilityStatusChanged, const FGameplayTag&, const FGameplayTag&,int32);
DECLARE_MULTICAST_DELEGATE_FourParams(FAbilityEquipped, const FGameplayTag&, const FGameplayTag&,const FGameplayTag&,const FGameplayTag&);
DECLARE_MULTICAST_DELEGATE_OneParam(FDeactivatePassiveAbility, const FGameplayTag&);
DECLARE_MULTICAST_DELEGATE_TwoParams(FActivatePassiveEffect,const FGameplayTag&,bool );

UCLASS()
class MYAURA_API UAuraAbilitySystemComponent : public UAbilitySystemComponent
{
	GENERATED_BODY()

	
public:
	
	FEffectAssetTags EffectAssetTags;
	FAbilityGiven AbilitiesGivenDelegate;
	FAbilityStatusChanged AbilityStatusChanged;
	FAbilityEquipped AbilityEquipped;
	FDeactivatePassiveAbility DeactivatePassiveAbility;
	FActivatePassiveEffect ActivatePassiveEffect;
	bool bStartupAbilitiesGiven = false;
	
	void InitAbilityActorInfoSet();
	void AddCharacterAbilitiesFromSaveData(ULoadScreenSaveGame* SaveGame);
	void AddCharacterAbilities(const TArray<TSubclassOf<UGameplayAbility>>& StartupAbilities);
	void AddCharacterPassiveAbilities(const TArray<TSubclassOf<UGameplayAbility>>& StartupAbilities);
	
	void AbilityInputTagPressed(const FGameplayTag& InputTag);
	void AbilityInputTagHeld(const FGameplayTag& InputTag);
	void AbilityInputTagRelease(const FGameplayTag& InputTag);
	void ForEachAbility(const FForEachAbility& Delegate);
	
	static FGameplayTag GetAbilityTagFromSpec(const FGameplayAbilitySpec& Spec);
	static FGameplayTag GetInputTagFromSpec(const FGameplayAbilitySpec& Spec);
	static bool AbilityHasSlot(FGameplayAbilitySpec* Spec, const FGameplayTag& Slot);
	static FGameplayTag GetStatusFromSpec(const FGameplayAbilitySpec& Spec);
	FGameplayAbilitySpec* GetSpecFromAbilityTag(const FGameplayTag& AbilityTag);
	FGameplayAbilitySpec* GetSpecFromSlotTag(const FGameplayTag& Slot);
	FGameplayTag GetStatusFromAbilityTag(const FGameplayTag& AbilityTag);
	FGameplayTag GetInputTagFromAbilityTag(const FGameplayTag& AbilityTag);
	bool SlotIsEmpty(const FGameplayTag& Slot);
	bool AbilitySpecHasSlot(const FGameplayAbilitySpec& Spec, const FGameplayTag& Slot);
	static bool AbilitySpecHasAnySlot(const FGameplayAbilitySpec& Spec);
	void AssignSlotToAbility(FGameplayAbilitySpec& Spec, const FGameplayTag& Slot);
	bool GetDescriptionByAbilityTag(const FGameplayTag& AbilityTag, FString& OutDescription, FString& OutNextLevelDescription);
	bool IsPassiveAbility(const FGameplayAbilitySpec& Spec) const;
	void UpdateAttribute(const FGameplayTag& AttributeTag);
	void UpdateAbilityStatues(int32 Level);
	static void ClearSlot(FGameplayAbilitySpec* Spec);
	void ClearAbilityOfSlot(const FGameplayTag& Slot);

	
	UFUNCTION(NetMulticast,Reliable)
	void MulticastActivatePassiveEffect(const FGameplayTag& AbilityTag, bool bActivate);
	
	UFUNCTION(Server,Reliable)
	void ServerUpgradeAttribute(const FGameplayTag& AttributeTag);
	
	UFUNCTION(Server,Reliable)
	void ServerSpendSpellPoint(const FGameplayTag& AbilityTag);
	
	UFUNCTION(Server,Reliable)
	void ServerEquipAbility(const FGameplayTag& AbilityTag,const FGameplayTag& Slot);
	
	UFUNCTION(Client,Reliable)
	void ClientEquipAbility(const FGameplayTag& AbilityTag,const FGameplayTag& Status, const FGameplayTag& Slot,const FGameplayTag& PrevSlot);
protected:
	
	virtual void OnRep_ActivateAbilities() override;
	
	UFUNCTION(Client, Reliable)  //声明为客户端RPC函数，持续发送直到接收到
	void ClientEffectApplied(UAbilitySystemComponent* AbilitySystemComponent, 
		const FGameplayEffectSpec& EffectSpec, FActiveGameplayEffectHandle ActiveEffectHandle);
	
	UFUNCTION(Client, Reliable)  //声明为客户端RPC函数，持续发送直到接收到
	void ClientUpdateAbilityStatus(const FGameplayTag& AbilityTag, const FGameplayTag& StatusTag,int32 AbilityLevel);
	
	
private:
	
};
