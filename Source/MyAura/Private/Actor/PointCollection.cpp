


#include "Actor/PointCollection.h"

#include "AbilitySystem/BlueFuncLibrary.h"
#include "Kismet/KismetMathLibrary.h"


APointCollection::APointCollection()
{
 	PrimaryActorTick.bCanEverTick = false;
	
	Pt_0 = CreateDefaultSubobject<USceneComponent>("Pt_0");
	SetRootComponent(Pt_0);
	ImmutablePts.Add(Pt_0);
	
	Pt_1 = CreateDefaultSubobject<USceneComponent>("Pt_1");
	Pt_1->SetupAttachment(GetRootComponent());
	ImmutablePts.Add(Pt_1);
	
	Pt_2 = CreateDefaultSubobject<USceneComponent>("Pt_2");
	Pt_2->SetupAttachment(GetRootComponent());
	ImmutablePts.Add(Pt_2);
	
	Pt_3 = CreateDefaultSubobject<USceneComponent>("Pt_3");
	Pt_3->SetupAttachment(GetRootComponent());
	ImmutablePts.Add(Pt_3);
	
	Pt_4 = CreateDefaultSubobject<USceneComponent>("Pt_4");
	Pt_4->SetupAttachment(GetRootComponent());
	ImmutablePts.Add(Pt_4);
	
	Pt_5 = CreateDefaultSubobject<USceneComponent>("Pt_5");
	Pt_5->SetupAttachment(GetRootComponent());
	ImmutablePts.Add(Pt_5);
}

TArray<USceneComponent*> APointCollection::GetGroundPoints(const FVector& GroundLocation, int32 NumPoints,
	float YawOverride)
{
	//判断条件是否满足
	checkf(ImmutablePts.Num()>=NumPoints,TEXT("请求点过多"));
	TArray<USceneComponent*> ArrayCopy;
	
	for (USceneComponent* Pt : ImmutablePts)
	{
		if (ArrayCopy.Num()>=NumPoints) return ArrayCopy;
		
		//绕pt -- p0旋转YawOverride，旋转点为P0
		if (Pt!=Pt_0)
		{
			FVector ToPoint  = Pt->GetComponentLocation() - Pt_0->GetComponentLocation();
			ToPoint = ToPoint.RotateAngleAxis(YawOverride,FVector::UpVector);
			Pt->SetWorldLocation(Pt_0->GetComponentLocation() + ToPoint);
		}
		
		//执行直线追踪，得到距离地面最近的点
		const FVector RaisedLocation =Pt->GetComponentLocation()+FVector(0,0,500);
		const FVector LoweredLocation =Pt->GetComponentLocation()-FVector(0,0,500);
		
		//寻找周围的敌人，进行直线追踪的时候忽略敌人
		TArray<AActor*> IgnoreActors;
		UBlueFuncLibrary::GetLivePlayerWithRadius(this,IgnoreActors,TArray<AActor*>(),1500,GetActorLocation());
		
		//调整点的高度
		FHitResult HitResult;
		FCollisionQueryParams CollisionParams;
		CollisionParams.AddIgnoredActors(IgnoreActors);
		GetWorld()->LineTraceSingleByProfile(HitResult,RaisedLocation,LoweredLocation,FName("BlockAll"),CollisionParams);
		const FVector AdjustedLocation = FVector(Pt->GetComponentLocation().X,Pt->GetComponentLocation().Y,HitResult.ImpactPoint.Z);
		Pt->SetWorldLocation(AdjustedLocation);
		Pt->SetWorldRotation(UKismetMathLibrary::MakeRotFromZ(HitResult.ImpactNormal));
			
		ArrayCopy.Add(Pt);
	}
	
	return ArrayCopy;
}



void APointCollection::BeginPlay()
{
	Super::BeginPlay();
	
}



