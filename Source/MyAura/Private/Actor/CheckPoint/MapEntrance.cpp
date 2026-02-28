


#include "Actor/CheckPoint/MapEntrance.h"

#include "Components/SphereComponent.h"
#include "GameMode/AuraGameModeBase.h"
#include "Interaction/PlayerInterface.h"
#include "Kismet/GameplayStatics.h"

AMapEntrance::AMapEntrance(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	Sphere->SetupAttachment(MoveToComponent);
}

void AMapEntrance::LoadActor_Implementation()
{
	// 什么都不需要做
}

void AMapEntrance::HighlightActor_Implementation()
{
	CheckpointMesh->SetRenderCustomDepth(true);
}

void AMapEntrance::OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OhterComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor->Implements<UPlayerInterface>())
	{
		bReached = true;  //需要在保存之前更新值，以便序列化
		if (const AAuraGameModeBase* AuraGM = Cast<AAuraGameModeBase>(UGameplayStatics::GetGameMode(this)))
		{
			//TODO: 保存下一个世界
			AuraGM->SaveWorldState(GetWorld(),DestinationMap.ToSoftObjectPath().GetAssetName());
		}
		IPlayerInterface::Execute_SaveProgress(OtherActor,DestinationPlayerStartTag);  //这是Check的point
		
		UGameplayStatics::OpenLevelBySoftObjectPtr(this,DestinationMap);
	}
}


