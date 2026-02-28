


#include "AbilitySystem/AsyncTasks/WaitCooldownChange.h"

#include "AbilitySystemComponent.h"

UWaitCooldownChange* UWaitCooldownChange::CreateCooldownChange(UAbilitySystemComponent* InASC,
                                                               const FGameplayTag& InCooldownTag)
{
	UWaitCooldownChange* WaitCooldownChange = NewObject<UWaitCooldownChange>();
	WaitCooldownChange->ASC = InASC;
	WaitCooldownChange->CooldownTag = InCooldownTag;
	if (!IsValid(InASC) || !InCooldownTag.IsValid())
	{
		WaitCooldownChange->EndTask();
		return nullptr;
	}
	
	//y用于通知冷却结束
	//监听游戏标签的变化
	InASC->RegisterGameplayTagEvent(InCooldownTag,EGameplayTagEventType::NewOrRemoved)
		.AddUObject(WaitCooldownChange,&UWaitCooldownChange::CooldownTagChanged);
	
	//用于通知冷却开始
	//Only for duration GE, called on server and client
	InASC->OnActiveGameplayEffectAddedDelegateToSelf.AddUObject(WaitCooldownChange,&UWaitCooldownChange::OnActivateEffectAdded);
	
	return WaitCooldownChange;
}

void UWaitCooldownChange::EndTask()
{
	if (! IsValid(ASC)) return ;
	ASC->RegisterGameplayTagEvent(CooldownTag,EGameplayTagEventType::NewOrRemoved).RemoveAll(this);
	
	SetReadyToDestroy(); 
	MarkAsGarbage();
}

void UWaitCooldownChange::CooldownTagChanged(const FGameplayTag InCooldownTag, int32 NewCount)
{
	if (NewCount == 0)
	{
		//Cooldown is End
		CooldownEnd.Broadcast(0.f);
	}
}

void UWaitCooldownChange::OnActivateEffectAdded(UAbilitySystemComponent* TargetASC,
	const FGameplayEffectSpec& SpecApplied, FActiveGameplayEffectHandle ActiveEffectHandle)
{
	FGameplayTagContainer AssetTag;
	SpecApplied.GetAllAssetTags(AssetTag);
	
	FGameplayTagContainer GrantedTag;
	SpecApplied.GetAllGrantedTags(GrantedTag);
	
	//这个GE拥有冷却标签，并且要授予目标ASC一个冷却标签
	if (AssetTag.HasTagExact(CooldownTag) || GrantedTag.HasTagExact(CooldownTag))
	{
		FGameplayEffectQuery GEQuery = FGameplayEffectQuery::MakeQuery_MatchAnyOwningTags(CooldownTag.GetSingleTagContainer());;
		TArray<float> TimesRemaining = ASC->GetActiveEffectsTimeRemaining(GEQuery);
		if (!TimesRemaining.IsEmpty())
		{
			//这里是因为我们只有一个持续GE
			float TimeRemaining = TimesRemaining[0];
			//可以寻找最大时间
			CooldownStart.Broadcast(TimeRemaining);
		}

	}
	
	
	
}
