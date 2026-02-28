


#include "AI/BTService_FindNearestPlayer.h"

#include "AIController.h"
#include "BehaviorTree/BTFunctionLibrary.h"
#include "Kismet/GameplayStatics.h"

void UBTService_FindNearestPlayer::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);
	
	// //这里的所有者是行为树组件的所有者不是行为树的所有者，行为树组件挂载到自定义AI控制器中
	// GEngine->AddOnScreenDebugMessage(1,1.f,FColor::Red,*AIOwner->GetName());
	// GEngine->AddOnScreenDebugMessage(2,1.f,FColor::Green,*ActorOwner->GetName());
	
	//寻找最近的玩家
	APawn* OwningPawn = AIOwner->GetPawn();
	const FName TargetTag = OwningPawn->ActorHasTag(FName("Player")) ? FName("Enemy") : FName("Player");
	
	TArray<AActor*> ActorsWithTargetTag;
	UGameplayStatics::GetAllActorsWithTag(OwningPawn,TargetTag,ActorsWithTargetTag);
	
	float ClosestDistance = TNumericLimits<float>::Max();
	AActor* ClosestActor = nullptr;
	for (AActor* Actor : ActorsWithTargetTag)
	{
		
		if (IsValid(Actor) && IsValid(OwningPawn))
		{
			const float Distance = OwningPawn->GetDistanceTo(Actor);
			if (Distance < ClosestDistance)
			{
				ClosestDistance = Distance;
				ClosestActor = Actor;
			}
		}
	}
	//为黑板键赋值
	UBTFunctionLibrary::SetBlackboardValueAsObject(this,Target2FollowSelector,ClosestActor);
	UBTFunctionLibrary::SetBlackboardValueAsFloat(this,Distance2FollowSelector,ClosestDistance);
}
