

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerStart.h"
#include "Interaction/HighLightInterface.h"
#include "Interaction/SaveInterface.h"
#include "CheckPoint.generated.h"

class USphereComponent;
/**
 * 
 */
UCLASS()
class MYAURA_API ACheckPoint : public APlayerStart,public ISaveInterface,public IHighLightInterface
{
	GENERATED_BODY()
public:
	ACheckPoint(const FObjectInitializer& ObjectInitializer);
	
	//Begin SaveInterface
	virtual bool ShouldLoadTransform_Implementation() override {return false;}
	virtual void LoadActor_Implementation() override;
	//End SaveInterface
	
	//highLight Interface
	virtual void SetMoveToLocation_Implementation(FVector& OutDestination) override;
	virtual void HighlightActor_Implementation() override;
	virtual void UnHighlightActor_Implementation() override;
	//end
	
	UPROPERTY(BlueprintReadWrite, SaveGame)
	bool bReached = false;
	
	UPROPERTY(EditAnywhere)
	bool bCallOverlapCallback = true;
protected:
	UFUNCTION()
	virtual void OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OhterComp,int32 OtherBodyIndex, bool bFromSweep,const FHitResult& SweepResult);
		
	UFUNCTION(BlueprintImplementableEvent)
	void CheckpointReached(UMaterialInstanceDynamic* DynamicMaterialInstance);
	
	virtual void BeginPlay() override;
	
	UFUNCTION(BlueprintCallable)
	void HandleGlowEffect();
	

	
	UPROPERTY(VisibleAnywhere,BlueprintReadOnly)
	TObjectPtr<UStaticMeshComponent> CheckpointMesh;
	
	UPROPERTY(VisibleAnywhere,BlueprintReadOnly)
	TObjectPtr<USceneComponent> MoveToComponent;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USphereComponent> Sphere;
};

