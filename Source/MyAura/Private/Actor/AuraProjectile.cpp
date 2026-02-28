


#include "Actor/AuraProjectile.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "AbilitySystem/BlueFuncLibrary.h"
#include "Components/AudioComponent.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "MyAura/MyAura.h"


AAuraProjectile::AAuraProjectile()
{
 	PrimaryActorTick.bCanEverTick = false;
	bReplicates = true;

	
	Sphere = CreateDefaultSubobject<USphereComponent>("Sphere");
	SetRootComponent(Sphere);
	Sphere->SetCollisionObjectType(ECC_Projectile);
	Sphere->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	Sphere->SetCollisionResponseToAllChannels(ECR_Ignore);
	Sphere->SetCollisionResponseToChannel(ECC_WorldDynamic,ECR_Overlap);
	Sphere->SetCollisionResponseToChannel(ECC_WorldStatic,ECR_Overlap);
	Sphere->SetCollisionResponseToChannel(ECC_Pawn,ECR_Overlap);
	
	ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>("ProjectileMovement");
	ProjectileMovementComponent->InitialSpeed = 550.f;
	ProjectileMovementComponent->MaxSpeed = 550.f;
	ProjectileMovementComponent->ProjectileGravityScale = 0.f;
}


void AAuraProjectile::BeginPlay()
{
	Super::BeginPlay();
	SetLifeSpan(LifeSpan);
	SetReplicateMovement(true);
	Sphere->OnComponentBeginOverlap.AddDynamic(this,&AAuraProjectile::OnSphereOverlap);
	
	LoopingSoundComponent = UGameplayStatics::SpawnSoundAttached(LoopingSound,GetRootComponent());
}

void AAuraProjectile::OnHit()
{
	UGameplayStatics::PlaySoundAtLocation(this,ImpactSound,GetActorLocation(),FRotator::ZeroRotator);
	UNiagaraFunctionLibrary::SpawnSystemAtLocation(this,ImpactEffect,GetActorLocation());
	if (LoopingSoundComponent)
	{
		LoopingSoundComponent->Stop();
		LoopingSoundComponent->DestroyComponent();
	}
	
	bHit=true;
}

//如果服务端的摧毁消息先于重叠回调函数执行，也能保证声音和特效播放成功
void AAuraProjectile::Destroyed()
{
	if (LoopingSoundComponent)
	{
		LoopingSoundComponent->Stop();
		LoopingSoundComponent->DestroyComponent();
	}
	if (!bHit && !HasAuthority())
	{
		OnHit();
	}
	OverlapActor.Reset(); 
	Super::Destroyed();
}

void AAuraProjectile::OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                      UPrimitiveComponent* OhterComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OverlapActor.Find(OtherActor))
		return;
	OverlapActor.Add(OtherActor);
	
	if (!IsValidOverlap(OtherActor)) return;
	
	//第一次击中播放声音
	if (!bHit)
	{
		OnHit();
	}
	
	if (HasAuthority())
	{
		//只有服务端才执行GE
		if (UAbilitySystemComponent* TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(OtherActor))
		{
			const FVector DeathImpulse = GetActorForwardVector() * DamageEffectParams.DeathImpulseMagnitude;
			DamageEffectParams.DeathImpulse = DeathImpulse;
			const bool bKnockBack = FMath::RandRange(1,100)<DamageEffectParams.KnockBackChance;
			if (bKnockBack)
			{
				FRotator Rotation = GetActorRotation();
				Rotation.Pitch = 45.f;
				
				const FVector KnockBackDirection = Rotation.Vector();
				const FVector KnockBackForce = KnockBackDirection * DamageEffectParams.KnockBackForceMagnitude;
				DamageEffectParams.KnockBackForce = KnockBackForce;
			}
			
			DamageEffectParams.TargetASC = TargetASC;
			UBlueFuncLibrary::ApplyDamageEffect(DamageEffectParams);
		}

		Destroy();  
	}
	else
	{
		bHit=true;
	}
}

bool AAuraProjectile::IsValidOverlap(AActor* OtherActor)
{
	if (DamageEffectParams.SourceASC == nullptr) return false;
	
	AActor* SourceAvatarActor = DamageEffectParams.SourceASC->GetAvatarActor();
	//如果和角色本身碰撞，直接返回
	if (SourceAvatarActor == OtherActor)  return false;
	
	//如果是友军直接返回
	if (!UBlueFuncLibrary::IsNotFriend(SourceAvatarActor,OtherActor)) return false;

	return true;
}



