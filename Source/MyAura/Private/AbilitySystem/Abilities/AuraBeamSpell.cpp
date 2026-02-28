


#include "AbilitySystem/Abilities/AuraBeamSpell.h"

#include "AbilitySystem/BlueFuncLibrary.h"
#include "GameFramework/Character.h"
#include "Kismet/KismetSystemLibrary.h"

void UAuraBeamSpell::StoreMouseDataInfo(const FHitResult& HitResult)
{
	if (HitResult.bBlockingHit)
	{
		MouseHitLocation = HitResult.ImpactPoint;
		MouseHitActor = HitResult.GetActor();
	}
	else
	{
		EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
	}
}

void UAuraBeamSpell::StoreOwnerInfos()
{
	if (CurrentActorInfo)
	{
		OwnerPlayerController = CurrentActorInfo->PlayerController.Get();
		OwnerCharacter = Cast<ACharacter>(CurrentActorInfo->AvatarActor);
	}
}

void UAuraBeamSpell::TraceFirstTarget(const FVector& BeamTargetLocation)
{
	check(OwnerCharacter);
	if (OwnerCharacter->Implements<UCombatInterface>())
	{
		if (USkeletalMeshComponent* Weapon = ICombatInterface::Execute_GetWeapon(OwnerCharacter))
		{
			TArray<AActor*> ActorsToIgnores;
			ActorsToIgnores.Add(OwnerCharacter);
			const FVector SocketLocation = Weapon->GetSocketLocation(FName("TipSocket"));
			FHitResult HitResult;
			UKismetSystemLibrary::SphereTraceSingle(OwnerCharacter,
				SocketLocation,
				BeamTargetLocation,
				10.f,
				TraceTypeQuery1,
				false,
				ActorsToIgnores,
				EDrawDebugTrace::None,
				HitResult,
				true);
			if (HitResult.bBlockingHit)
			{
				MouseHitLocation = HitResult.ImpactPoint;
				MouseHitActor = HitResult.GetActor();
			}
		}
	}
	
	if (ICombatInterface* CombatInterface = Cast<ICombatInterface>(MouseHitActor))
	{
		if (!CombatInterface->GetOnDeathDelegate2().IsAlreadyBound(this,&UAuraBeamSpell::PrimaryTargetDied))
		{
			CombatInterface->GetOnDeathDelegate2().AddDynamic(this,&UAuraBeamSpell::PrimaryTargetDied);
		}
	}
}

void UAuraBeamSpell::StoreAdditionalTargets(TArray<AActor*>& OutAdditionalTargetActors)
{
	TArray<AActor*> OverlappingActors;
	
	TArray<AActor*> ActorsToIgnore;
	ActorsToIgnore.Add(GetAvatarActorFromActorInfo());
	ActorsToIgnore.Add(MouseHitActor);
	
	//获取击中目标周围范围内的所有角色
	UBlueFuncLibrary::GetLivePlayerWithRadius(
		GetAvatarActorFromActorInfo(),
		OverlappingActors,
		ActorsToIgnore,
		850,
		MouseHitActor->GetActorLocation());

	const int32 NumAdditionalTargets = FMath::Min(GetAbilityLevel()-1,MaxNumShockTargets);
	
	if (NumAdditionalTargets > 0)
	{
		UBlueFuncLibrary::GetClosestTarget(
			NumAdditionalTargets,
			OverlappingActors,
			OutAdditionalTargetActors,
			MouseHitActor->GetActorLocation());
	
		for (AActor* Actor : OutAdditionalTargetActors)
		{
			if (ICombatInterface* CombatInterface = Cast<ICombatInterface>(Actor))
			{
				if (!CombatInterface->GetOnDeathDelegate2().IsAlreadyBound(this,&UAuraBeamSpell::AdditionalTargetDied))
				{
					CombatInterface->GetOnDeathDelegate2().AddDynamic(this,&UAuraBeamSpell::AdditionalTargetDied);
				}
			}
		}
	}
}

