


#include "UI/WidgetController/SpellMenuWidgetController.h"

#include "AuraGameplayTags.h"
#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "AbilitySystem/Data/AbilityInfo.h"
#include "Player/AuraPlayerState.h"

void USpellMenuWidgetController::BroadcastInitialValue()
{
	//广播已经装备的初始能力
	BroadcastAbilityInfo();
	CurrentSpellPints = GetAuraPS()->GetSpellPoints();
	SpellPointChanged.Broadcast(GetAuraPS()->GetSpellPoints());

}

void USpellMenuWidgetController::BindCallbacksToDependencies()
{	
	//再Hud中第一次创建controller的时候绑定
	GetAuraASC()->AbilityStatusChanged.AddLambda([this](const FGameplayTag& AbilityTag,const FGameplayTag& StatusTag,int32 NewLevel)
	{
		//边缘情况，如果选中能力并且能力状态更改后，需要修改消耗技能点和装备按钮的状态
		if (AbilityTag.MatchesTagExact(SelectAbility.AbilityTag))
		{
			SelectAbility.StatusTag = StatusTag;
			bool bSpend = false;
			bool bEquip = false;
			ShouldEnableButtons(SelectAbility.StatusTag,CurrentSpellPints,bSpend,bEquip);
			FString Description;
			FString NextLevelDescription;
			GetAuraASC()->GetDescriptionByAbilityTag(AbilityTag,Description,NextLevelDescription);
			SpellGlobeSelectedDelegate.Broadcast(bSpend,bEquip,Description,NextLevelDescription);
			
		}
		if (AbilityInfo)
		{
			FAuraAbilityInfo Info = AbilityInfo->FindAbilityInfoByTag(AbilityTag);
			Info.StatusTag = StatusTag;
			AbilityInfoDelegate.Broadcast(Info);
		}
	});
	
	GetAuraASC()->AbilityEquipped.AddUObject(this,&USpellMenuWidgetController::OnAbilityEquipped);
	
	GetAuraPS()->OnSpellPointsChangedDelegate.AddLambda([this](int32 SpellPoints)
	{
		SpellPointChanged.Broadcast(SpellPoints);
		CurrentSpellPints = SpellPoints;

		//边缘情况，如果选中能力并且升级后，需要修改消耗技能点和装备按钮的状态
		bool bSpend = false;
		bool bEquip = false;
		ShouldEnableButtons(SelectAbility.StatusTag,CurrentSpellPints,bSpend,bEquip);
		FString Description;
		FString NextLevelDescription;
		GetAuraASC()->GetDescriptionByAbilityTag(SelectAbility.AbilityTag,Description,NextLevelDescription);
		SpellGlobeSelectedDelegate.Broadcast(bSpend,bEquip,Description,NextLevelDescription);
			
	});
}

//在Widget中调用该函数
void USpellMenuWidgetController::SpellGlobeSelected(const FGameplayTag& AbilityTag)
{
	if (bWaiteForEquipSelection)
	{
		//停止播放所有动画，并播放当前选择的技能类型的动画（该动画仅播放一次，不同于装备按钮按下后的循环动画）
		FGameplayTag AbilityType = AbilityInfo->FindAbilityInfoByTag(AbilityTag).AbilityType;
		StopWaiteForEquipSelectionDelegate.Broadcast(AbilityType);
		bWaiteForEquipSelection = false;
	}

	
	const int32 SpellPoints = GetAuraPS()->GetSpellPoints();
	FGameplayTag AbilityStatus;
	
	const FAuraGameplayTags GameplayTags =FAuraGameplayTags::Get();
	bool bTagValid = AbilityTag.IsValid();
	bool bTagNone = AbilityTag.MatchesTag(GameplayTags.Abilities_None);
	FGameplayAbilitySpec* AbilitySpec = GetAuraASC()->GetSpecFromAbilityTag(AbilityTag);
	bool bSpecValid = AbilitySpec!=nullptr;
	
	if (!bTagValid || bTagNone || !bSpecValid)
	{
		AbilityStatus = GameplayTags.Abilities_Status_Locked;
	}
	else
	{
		AbilityStatus = GetAuraASC()->GetStatusFromSpec(*AbilitySpec);
	}
	SelectAbility.AbilityTag = AbilityTag;
	SelectAbility.StatusTag = AbilityStatus;
	
	bool bSpend = false;
	bool bEquip = false;
	ShouldEnableButtons(AbilityStatus,SpellPoints,bSpend,bEquip);
	FString Description;
	FString NextLevelDescription;
	GetAuraASC()->GetDescriptionByAbilityTag(AbilityTag,Description,NextLevelDescription);
	SpellGlobeSelectedDelegate.Broadcast(bSpend,bEquip,Description,NextLevelDescription);


}

void USpellMenuWidgetController::SpendPointButtonPressed()
{
	//传递信息给ASC
	if (GetAuraASC())
	{
		GetAuraASC()->ServerSpendSpellPoint(SelectAbility.AbilityTag);
	}
}

void USpellMenuWidgetController::GlobeDeselect()
{
	if (bWaiteForEquipSelection)
	{
		FGameplayTag AbilityType = AbilityInfo->FindAbilityInfoByTag(SelectAbility.AbilityTag).AbilityType;
		StopWaiteForEquipSelectionDelegate.Broadcast(AbilityType);
		bWaiteForEquipSelection = false;
	}

	SelectAbility.AbilityTag = FAuraGameplayTags::Get().Abilities_None;
	SelectAbility.StatusTag = FAuraGameplayTags::Get().Abilities_Status_Locked;
	SpellGlobeSelectedDelegate.Broadcast(false,false,FString(),FString());
}

void USpellMenuWidgetController::EquipButtonPressed()
{
	const FGameplayTag AbilityType = AbilityInfo->FindAbilityInfoByTag(SelectAbility.AbilityTag).AbilityType;
	WaiteForEquipSelectionDelegate.Broadcast(AbilityType);
	bWaiteForEquipSelection = true;
	
	// const FGameplayTag SelectStatus = GetAuraASC()->GetStatusFromAbilityTag(SelectAbility.AbilityTag);
	// if (SelectStatus.MatchesTag(FAuraGameplayTags::Get().Abilities_Status_Equipped))
	// {
	// 	SelectAbility.SelectedSlot = GetAuraASC()->
	// }
	
	//如果选中的是已经装备的能力，需要保存能力的输入标签
	if (SelectAbility.StatusTag.MatchesTag(FAuraGameplayTags::Get().Abilities_Status_Equipped))
	{
		SelectAbility.SelectedSlot = GetAuraASC()->GetInputTagFromAbilityTag(SelectAbility.AbilityTag);
	}
	
	
}

void USpellMenuWidgetController::SpellRowGlobePressed(const FGameplayTag& SlotTag, const FGameplayTag& AbilityType)
{
	if (!bWaiteForEquipSelection) return;  //如果没有事先点击装备按钮则直接返回
	//	只有当所选技能的类型和装备技能槽的所需的技能类型一致才可以装备
	//  即主动技能只能装备到主动技能潮槽，被动技能同理
	const FGameplayTag& SelectedAbilityType = AbilityInfo->FindAbilityInfoByTag(SelectAbility.AbilityTag).AbilityType;
	if (!SelectedAbilityType.MatchesTagExact(AbilityType)) return;

	GetAuraASC()->ServerEquipAbility(SelectAbility.AbilityTag,SlotTag);
}

void USpellMenuWidgetController::OnAbilityEquipped(const FGameplayTag& AbilityTag, const FGameplayTag& Status,
	const FGameplayTag& Slot, const FGameplayTag& PrevSlot)
{
	bWaiteForEquipSelection = false;
	const FAuraGameplayTags& GameplayTags = FAuraGameplayTags::Get();
	
	FAuraAbilityInfo LastSlotInfo;
	LastSlotInfo.AbilityTag = GameplayTags.Abilities_None;
	LastSlotInfo.StatusTag = GameplayTags.Abilities_Status_Unlocked;
	LastSlotInfo.InputTag = PrevSlot;
	AbilityInfoDelegate.Broadcast(LastSlotInfo);
	
	FAuraAbilityInfo Info = AbilityInfo->FindAbilityInfoByTag(AbilityTag);
	Info.StatusTag = Status;
	Info.InputTag = Slot;
	AbilityInfoDelegate.Broadcast(Info);
	
	StopWaiteForEquipSelectionDelegate.Broadcast(Info.AbilityType);
	SpellGlobeReassignedDelegate.Broadcast(AbilityTag);
	GlobeDeselect();  //禁用按钮，清楚描述框
}

void USpellMenuWidgetController::ShouldEnableButtons(const FGameplayTag& AbilityStatus, int32 SpellPoints,
                                                     bool& bSpend, bool& bEquip)
{
	const FAuraGameplayTags GameplayTags =FAuraGameplayTags::Get();
	
	if (AbilityStatus.MatchesTag(GameplayTags.Abilities_Status_Equipped))
	{
		bEquip = true;
		if (SpellPoints>0)
		{
			bSpend = true;
		}
	}
	else if (AbilityStatus.MatchesTag(GameplayTags.Abilities_Status_Eligible))
	{
		bEquip = false;
		if (SpellPoints>0)
		{
			bSpend = true;
		}
	}
	else if (AbilityStatus.MatchesTag(GameplayTags.Abilities_Status_Unlocked))
	{
		bEquip = true;
		if (SpellPoints>0)
		{
			bSpend = true;
		}
	}
	else if (AbilityStatus.MatchesTag(GameplayTags.Abilities_Status_Locked))
	{
		bEquip = false;
		if (SpellPoints>0)
		{
			bSpend = false;
		}
	}
}
