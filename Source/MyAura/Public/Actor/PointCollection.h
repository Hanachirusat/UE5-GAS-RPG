

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PointCollection.generated.h"

UCLASS()
class MYAURA_API APointCollection : public AActor
{
	GENERATED_BODY()
	
public:	
	APointCollection();
	
	UFUNCTION(BlueprintPure)
	TArray<USceneComponent*> GetGroundPoints(const FVector& GroundLocation, int32 NumPoints, float YawOverride = 0.f);

protected:
	virtual void BeginPlay() override;
	
	UPROPERTY(BlueprintReadOnly)
	TArray<USceneComponent*> ImmutablePts;
	
	UPROPERTY(BlueprintReadWrite,visibleAnywhere)
	TObjectPtr<USceneComponent> Pt_0;
	
	UPROPERTY(BlueprintReadWrite,visibleAnywhere)
	TObjectPtr<USceneComponent> Pt_1;
	
	UPROPERTY(BlueprintReadWrite,visibleAnywhere)
	TObjectPtr<USceneComponent> Pt_2;
	
	UPROPERTY(BlueprintReadWrite,visibleAnywhere)
	TObjectPtr<USceneComponent> Pt_3;
	
	UPROPERTY(BlueprintReadWrite,visibleAnywhere)
	TObjectPtr<USceneComponent> Pt_4;
	
	UPROPERTY(BlueprintReadWrite,visibleAnywhere)
	TObjectPtr<USceneComponent> Pt_5;
};
