


#include "AbilitySystem/Abilities/FireBlast.h"

#include "AbilitySystem/BlueFuncLibrary.h"
#include "Actor/AuraFireBall.h"

FString UFireBlast::GetDescription(int32 Level)
{
	const int ScaledDamage = Damage.GetValueAtLevel(Level);
	const float ManaCost = FMath::Abs(GetManaCost(Level));
	const float Cooldown = GetCooldown(Level);

	return FString::Printf(TEXT(
		"<Title>烈火爆</>\n"
		
		"<Small>等级：</><Level>%d</>\n\n"
		
		"<Default>召唤</><Damage>%d</><Default>个火球射向四周，"
		"火球爆炸后造成</><Damage>%d</><Default>点火焰伤害，有一定概率使敌人烧伤（持续损失生命值）</>\n\n"
		
		"<Small>法力值消耗：</><ManaCost>%.1f</>\n"
		"<Small>冷却时间：</><Cooldown>%.1f</>\n"),
		Level,
		NumFireBall,
		ScaledDamage,
		ManaCost,
		Cooldown);
}

FString UFireBlast::GetNextLevelDescription(int32 Level)
{
	const int ScaledDamage = Damage.GetValueAtLevel(Level);
	const float ManaCost = FMath::Abs(GetManaCost(Level));
	const float Cooldown = GetCooldown(Level);
	return FString::Printf(TEXT(
		"<Title>烈火爆</>\n"
		
		"<Small>下一等级：</><Level>%d</>\n\n"
		
		"<Default>召唤</><Damage>%d</><Default>个火球射向四周，"
		"火球爆炸后造成</><Damage>%d</><Default>点火焰伤害，有一定概率使敌人烧伤（持续损失生命值）</>\n\n"
		
		"<Small>法力值消耗：</><ManaCost>%.1f</>\n"
		"<Small>冷却时间：</><Cooldown>%.1f</>\n"),
		Level,
		NumFireBall,
		ScaledDamage,
		ManaCost,
		Cooldown);

}

TArray<AAuraFireBall*> UFireBlast::SpawnFireBalls()
{
	TArray<AAuraFireBall*> FireBalls;
	const FVector Forward = GetAvatarActorFromActorInfo()->GetActorForwardVector();
	const FVector Location = GetAvatarActorFromActorInfo()->GetActorLocation();
	TArray<FRotator> Rotators = UBlueFuncLibrary::EvenlyRotators(Forward,FVector::UpVector,360.f,NumFireBall);
	
	for (const FRotator& Rotator : Rotators)
	{
		FTransform SpawnTransform;
		SpawnTransform.SetLocation(Location);
		SpawnTransform.SetRotation(Rotator.Quaternion());  //这里函数要求四元组
		AAuraFireBall* FireBall = GetWorld()->SpawnActorDeferred<AAuraFireBall>(
			FireBallClass,
			SpawnTransform,
			GetOwningActorFromActorInfo(),
			CurrentActorInfo->PlayerController->GetPawn(),
			ESpawnActorCollisionHandlingMethod::AlwaysSpawn);
		
		FireBall->DamageEffectParams = MakeDamageEffectParamsFromClassDefaults();
		FireBall->ReturnToActor = GetAvatarActorFromActorInfo();
		//有一些无法再这里设置
		FireBall->ExplosionDamageParams = MakeDamageEffectParamsFromClassDefaults();
		
		
		FireBall->FinishSpawning(SpawnTransform);
		FireBalls.Add(FireBall);
	}
	
	return FireBalls;
}
