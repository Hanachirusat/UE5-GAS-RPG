


#include "Actor/MagicCircle.h"

#include "Components/DecalComponent.h"

AMagicCircle::AMagicCircle()
{
 	PrimaryActorTick.bCanEverTick = true;
	
	MagicCircleComponent = CreateDefaultSubobject<UDecalComponent>("MagicCircleComponent");;
	MagicCircleComponent->SetupAttachment(GetRootComponent());

}


void AMagicCircle::BeginPlay()
{
	Super::BeginPlay();
	
}


void AMagicCircle::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

