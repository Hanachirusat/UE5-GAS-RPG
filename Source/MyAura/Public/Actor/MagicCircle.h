

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MagicCircle.generated.h"

UCLASS()
class MYAURA_API AMagicCircle : public AActor
{
	GENERATED_BODY()
	
public:	
	
	AMagicCircle();
	virtual void Tick(float DeltaTime) override;
	
	UPROPERTY(VisibleAnywhere,BlueprintReadOnly)
	TObjectPtr<UDecalComponent> MagicCircleComponent;
	
protected:
	virtual void BeginPlay() override;

};
