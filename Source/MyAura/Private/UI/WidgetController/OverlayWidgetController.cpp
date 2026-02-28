 


#include "UI/WidgetController/OverlayWidgetController.h"

#include "AuraGameplayTags.h"
#include "GameplayEffectTypes.h"
#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "AbilitySystem/AuraAttributeSet.h"
#include "AbilitySystem/Data/AbilityInfo.h"
#include "AbilitySystem/Data/LevelUpInfo.h"
#include "Player/AuraPlayerState.h"

void UOverlayWidgetController::BroadcastInitialValue()
{
	
	OnHealthChanged.Broadcast(GetAuraAS()->GetHP());
	OnMaxHealthChanged.Broadcast(GetAuraAS()->GetMaxHP());
	OnManaChanged.Broadcast(GetAuraAS()->GetMana());
	OnMaxManaChanged.Broadcast(GetAuraAS()->GetMaxMana());
}

//在HUD中被调用
void UOverlayWidgetController::BindCallbacksToDependencies()
{
	/* 经验变化回调*/
	GetAuraPS()->OnXPChangedDelegate.AddUObject(this,&UOverlayWidgetController::OnXPChanged);
	GetAuraPS()->OnLevelChangedDelegate.AddLambda(
		[this](int32 NewLevel)
		{
			OnLevelChangedDelegate.Broadcast(NewLevel);
		}	
	);
	/* 属性变化回调*/
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
		GetAuraAS()->GetHPAttribute()).AddLambda(
			[this](const FOnAttributeChangeData& Date)
			{
				//属性变化的时候调用UI更新委托上的事件（该事件蓝图实现和绑定）
				OnHealthChanged.Broadcast(Date.NewValue);
			}	
		);
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
		GetAuraAS()->GetMaxHPAttribute()).AddLambda(
		[this](const FOnAttributeChangeData& Date)
			{
			    OnMaxHealthChanged.Broadcast(Date.NewValue);
			}	
		);
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
	GetAuraAS()->GetManaAttribute()).AddLambda(
	[this](const FOnAttributeChangeData& Date)
		{
			OnManaChanged.Broadcast(Date.NewValue);
		}	
	);
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
	GetAuraAS()->GetMaxManaAttribute()).AddLambda(
	[this](const FOnAttributeChangeData& Date)
		{
			OnMaxManaChanged.Broadcast(Date.NewValue);
		}	
	);
	
	if (GetAuraASC())
	{
		GetAuraASC()->AbilityEquipped.AddUObject(this,&UOverlayWidgetController::OnAbilityEquipped);
		/* TAG变化回调*/
		GetAuraASC()->EffectAssetTags.AddLambda(
[this](const FGameplayTagContainer& AssetTags)
		{
			for (const FGameplayTag& Tag : AssetTags)
			{
				//A.1".MatchesTag("A") will return True, "A".MatchesTag("A.1") will return False 
				FGameplayTag MessageTag = FGameplayTag::RequestGameplayTag(FName("Message"));
				if (Tag.MatchesTag(MessageTag))
				{
					FUIWidgetRow* Row = GetDataTableRowByTag<FUIWidgetRow>(MessageWidgetDataTable,Tag);
					MessageWidgetRowDelegate.Broadcast(*Row);
				}
			}
		});
		
		/* 初始化GA UI的回调*/
		if (GetAuraASC()->bStartupAbilitiesGiven)
		{
			BroadcastAbilityInfo();
		}
		else
		{
			GetAuraASC()->AbilitiesGivenDelegate.AddUObject(this,&UOverlayWidgetController::BroadcastAbilityInfo);
		}
	}
}




void UOverlayWidgetController::OnXPChanged(int32 NewXP)
{
	//广播一个浮点数
	const ULevelUpInfo* LevelUpInfo = GetAuraPS()->LevelUpInfo;
	checkf(LevelUpInfo,TEXT("找不到LevelUpInfo，请在蓝图中填充AuraPlayerState中的信息"));

	const int32 Level = LevelUpInfo->FindLevelForXP(NewXP);
	const int32 MaxLevel = LevelUpInfo->LevelUpInformation.Num();
	
	if (Level<=MaxLevel && Level>0)
	{
		const int32 LevelUpRequirement = LevelUpInfo->LevelUpInformation[Level].LevelUpRequirement;
		const int32 PreviousLevelUpRequirement = LevelUpInfo->LevelUpInformation[Level-1].LevelUpRequirement;
		
		//升到下一等级需要的经验
		const int32 Delta = LevelUpRequirement - PreviousLevelUpRequirement;
		const int32 XPForThisLevel = NewXP - PreviousLevelUpRequirement;  //当前等级的累计经验
		
		const float XPBarPercent = static_cast<float>(XPForThisLevel) / static_cast<float>(Delta);
		
		OnXPPercentChangedDelegate.Broadcast(XPBarPercent);
	}
}

void UOverlayWidgetController::OnAbilityEquipped(const FGameplayTag& AbilityTag, const FGameplayTag& Status,
	const FGameplayTag& Slot, const FGameplayTag& PrevSlot) const
{
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
}	








