

#pragma once

#include "CoreMinimal.h"
#include "Actor/CheckPoint/CheckPoint.h"
#include "MapEntrance.generated.h"

/**
 * 
 */
UCLASS()
class MYAURA_API AMapEntrance : public ACheckPoint
{
	GENERATED_BODY()
	
public:
	
	AMapEntrance(const FObjectInitializer& ObjectInitializer);
	//Begin SaveInterface
	virtual void LoadActor_Implementation() override;
	//End SaveInterface
	
	//highLight Interface
	virtual void HighlightActor_Implementation() override;
	//end
	
	UPROPERTY(EditAnywhere)
	TSoftObjectPtr<UWorld> DestinationMap;
	
	UPROPERTY(EditAnywhere)
	FName DestinationPlayerStartTag;
protected:
	
	virtual  void OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OhterComp,int32 OtherBodyIndex, bool bFromSweep,const FHitResult& SweepResult);
	
	

private:
	
};
