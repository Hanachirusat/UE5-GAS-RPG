


#include "AbilitySystem/Abilities/AuraFireBolt.h"

#include "AbilitySystem/BlueFuncLibrary.h"
#include "Actor/AuraProjectile.h"
#include "GameFramework/ProjectileMovementComponent.h"


FString UAuraFireBolt::GetDescription(int32 Level)
{
	const int ScaledDamage = Damage.GetValueAtLevel(Level);
	const float ManaCost = FMath::Abs(GetManaCost(Level));
	const float Cooldown = GetCooldown(Level);
	if (Level ==1)
	{
		return FString::Printf(TEXT(
			"<Title>火球术</>\n"
			
			"<Small>等级：</><Level>%d</>\n\n"
			
			"<Default>向指定方向发射</><Damage>1</><Default>个火球，"
			"火球爆炸后造成</><Damage>%d</><Default>点火焰伤害，有一定概率使敌人烧伤（持续损失生命值）</>\n\n"
			
			"<Small>法力值消耗：</><ManaCost>%.1f</>\n"
			"<Small>冷却时间：</><Cooldown>%.1f</>\n"),
			Level,
			ScaledDamage,
			ManaCost,
			Cooldown);
	}
	else
	{
		return FString::Printf(TEXT(
			"<Title>火球术</>\n"
					
			"<Small>等级：</><Level>%d</>\n\n"
					
			"<Default>向指定方向发射</><Damage>%d</><Default>个火球，"
			"火球爆炸后造成</><Damage>%d</><Default>点火焰伤害，，有一定概率使敌人烧伤（持续损失生命值）</>\n\n"
					
			"<Small>法力值消耗：</><ManaCost>%.1f</>\n"
			"<Small>冷却时间：</><Cooldown>%.1f</>\n"),
			Level,
			FMath::Min(Level,MaxNumProjectiles),
			ScaledDamage,
			ManaCost,
			Cooldown);
	}
}

FString UAuraFireBolt::GetNextLevelDescription(int32 Level)
{	
	const int ScaledDamage = Damage.GetValueAtLevel(Level);
	const float ManaCost = FMath::Abs(GetManaCost(Level));
	const float Cooldown = GetCooldown(Level);
	
	return FString::Printf(TEXT(
			"<Title>火球术</>\n"
					
			"<Small>下一等级：</><Level>%d</>\n\n"
					
			"<Default>向指定方向发射</><Damage>%d</><Default>个火球，"
			"火球爆炸后造成</><Damage>%d</><Default>点火焰伤害</>\n\n"
					
			"<Small>法力值消耗：</><ManaCost>%.1f</>\n"
			"<Small>冷却时间：</><Cooldown>%.1f</>\n"),
			Level,
			FMath::Min(Level,MaxNumProjectiles),
			ScaledDamage,
			ManaCost,
			Cooldown);
}

//专门 用于FireBoltSpread技能的函数
void UAuraFireBolt::SpawnProjectiles(const FVector& ProjectileTargetLocation, const FGameplayTag& SocketTag,
	bool bOverridePitch, float PitchOverride,  AActor* Homingtarget)
{
	//投掷物在服务端生成，生成的投掷物可复制到客户端
	const bool bIsServer = GetAvatarActorFromActorInfo()->HasAuthority();
	if (!bIsServer) return;

	const FVector SocketLocation = ICombatInterface::Execute_GetCombatSocketLocation(
		GetAvatarActorFromActorInfo(),
		SocketTag);
	
	FRotator Rotation = (ProjectileTargetLocation - SocketLocation).Rotation();
	if (bOverridePitch) Rotation.Pitch = PitchOverride;
	const FVector Forward = Rotation.Vector();
	
	TArray<FRotator> Rotators = UBlueFuncLibrary::EvenlyRotators(Forward,FVector::UpVector,ProjectileSpread,FMath::Min(MaxNumProjectiles,GetAbilityLevel()));
	
	for (const FRotator& Rotator : Rotators)
	{
		FTransform SpawnTransform;
		SpawnTransform.SetLocation(SocketLocation);
		SpawnTransform.SetRotation(Rotator.Quaternion());
		
		AAuraProjectile* Projectile = GetWorld()->SpawnActorDeferred<AAuraProjectile>(   //Deferred可以设置Actor的属性后在生成并初始化
			ProjectileClass,
			SpawnTransform,
			GetOwningActorFromActorInfo(),
			Cast<APawn>(GetOwningActorFromActorInfo()),
			ESpawnActorCollisionHandlingMethod::AlwaysSpawn);
	
		Projectile->DamageEffectParams = MakeDamageEffectParamsFromClassDefaults();
	

		//  处理追踪
		if (Homingtarget && Homingtarget->Implements<UCombatInterface>())
		{
			Projectile->ProjectileMovementComponent->HomingTargetComponent = Homingtarget->GetRootComponent();
		}
		else
		{
			Projectile->HomingTargetSceneComponent = NewObject<USceneComponent>(USceneComponent::StaticClass());
			Projectile->HomingTargetSceneComponent->SetWorldLocation(ProjectileTargetLocation);
			Projectile->ProjectileMovementComponent->HomingTargetComponent = Projectile->HomingTargetSceneComponent;
		}
		Projectile->ProjectileMovementComponent->HomingAccelerationMagnitude = HomingAcceleration;
		Projectile->ProjectileMovementComponent->bIsHomingProjectile = bIsHomingProjectile;
		Projectile->FinishSpawning(SpawnTransform);
	}
	
}
