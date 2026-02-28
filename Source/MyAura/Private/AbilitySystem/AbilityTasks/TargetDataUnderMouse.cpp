


#include "AbilitySystem/AbilityTasks/TargetDataUnderMouse.h"

#include "AbilitySystemComponent.h"
#include "MyAura/MyAura.h"

UTargetDataUnderMouse* UTargetDataUnderMouse::CreateTargetDataUnderMouse(UGameplayAbility* OwningAbility)
{
	UTargetDataUnderMouse* MyObj = NewAbilityTask<UTargetDataUnderMouse>(OwningAbility, "TargetDataUnderMouse");
	
	return MyObj;
}

//GA激活的时候TASK会激活，TASK激活的时候广播光标位置
void UTargetDataUnderMouse::Activate()
{
	Super::Activate();
	const bool bIsLocallyControlled = Ability->GetCurrentActorInfo()->IsLocallyControlled();
	if (bIsLocallyControlled)
	{
		//客户端
		SendTargetDataUnderMouse();
	}
	else
	{
		//服务端
		FGameplayAbilitySpecHandle SpecHandle = GetAbilitySpecHandle();
		FPredictionKey PredictionKey = GetActivationPredictionKey();
		//委托需要GA和GA原始的Predectionkey来确定。
		AbilitySystemComponent.Get()->AbilityTargetDataSetDelegate(
			SpecHandle, PredictionKey).Clear();
		AbilitySystemComponent.Get()->AbilityTargetDataSetDelegate(
			SpecHandle, PredictionKey).AddUObject(this,&UTargetDataUnderMouse::OnTargetDataReplicatedCallback);
		
		//如果数据来的早，就再广播一次,先判断是否广播过，如果没有就等待，如果存在就立即执行回调（bCallDelegate为true自动调用回调）
		const bool bCallDelegate = AbilitySystemComponent.Get()->CallReplicatedTargetDataDelegatesIfSet(SpecHandle,PredictionKey);
		if (!bCallDelegate)
		{
			SetWaitingOnRemotePlayerData();
			UE_LOG(LogTemp, Log, TEXT("Server: SetWaitingOnRemotePlayerData"));
		}
		else
		{
			UE_LOG(LogTemp, Log, TEXT("Server: Callback was already called"));
		}
	}
}

void UTargetDataUnderMouse::SendTargetDataUnderMouse()
{
	// 确保有有效的预测键
	FScopedPredictionWindow ScopedPrediction(AbilitySystemComponent.Get());  //后续范围内的所有操作都要预测
	
	//这里的数据是光标追踪后的HitResult
	APlayerController* PC = Ability->GetCurrentActorInfo()->PlayerController.Get();
	FHitResult CursotHit;
	PC->GetHitResultUnderCursor(ECC_Target,false, CursotHit);
	
	FGameplayAbilityTargetData_SingleTargetHit* Data = new FGameplayAbilityTargetData_SingleTargetHit();
	Data->HitResult = CursotHit;
	FGameplayAbilityTargetDataHandle DataHandle;
	DataHandle.Add(Data);
	
	AbilitySystemComponent->ServerSetReplicatedTargetData(
		GetAbilitySpecHandle(),
		GetActivationPredictionKey(),   //和GA相关的
		DataHandle,
		FGameplayTag(),
		AbilitySystemComponent->ScopedPredictionKey); //和该上下文相关的Key,这是个变量，我们需要自己更新
	
	//在某些情况下不应该广播，例如能力已经失效，因此再广播数据前应该先检查
	if (ShouldBroadcastAbilityTaskDelegates())
	{
		ValidData.Broadcast(DataHandle);
	}
}

void UTargetDataUnderMouse::OnTargetDataReplicatedCallback(const FGameplayAbilityTargetDataHandle& DataHandle,
	FGameplayTag ActivationTag)
{
	UE_LOG(LogTemp, Warning, TEXT(" 服务端执行回调函数"));
	//告诉ASC数据已经收到，不用缓存
	AbilitySystemComponent->ConsumeClientReplicatedTargetData(GetAbilitySpecHandle(),GetActivationPredictionKey());
	if (ShouldBroadcastAbilityTaskDelegates())
	{
		ValidData.Broadcast(DataHandle);
	}
}
