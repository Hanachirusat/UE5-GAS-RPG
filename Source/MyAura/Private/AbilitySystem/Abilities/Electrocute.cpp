


#include "AbilitySystem/Abilities/Electrocute.h"

FString UElectrocute::GetDescription(int32 Level)
{
	const int ScaledDamage = Damage.GetValueAtLevel(Level);
	const float ManaCost = FMath::Abs(GetManaCost(Level));
	const float Cooldown = GetCooldown(Level);
	if (Level ==1)
	{
		return FString::Printf(TEXT(
			"<Title>连锁闪电</>\n"
			
			"<Small>等级：</><Level>%d</>\n\n"
			
			"<Default>向指定方向发射闪电，"
			"闪电造成</><Damage>%d</><Default>点雷电伤害</>有一定概率使敌人麻痹（无法行动）\n\n"
			
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
			"<Title>连锁闪电</>\n"
					
			"<Small>等级：</><Level>%d</>\n\n"
					
			"<Default>向指定方向发射闪电，在击中敌人后额外分散</><Damage>%d</><Default>个子闪电，"
			"闪电造成</><Damage>%d</><Default>点雷电伤害</>有一定概率使敌人麻痹（无法行动）\n\n"
					
			"<Small>法力值消耗：</><ManaCost>%.1f</>\n"
			"<Small>冷却时间：</><Cooldown>%.1f</>\n"),
			Level,
			FMath::Min(Level-1,MaxNumShockTargets),
			ScaledDamage,
			ManaCost,
			Cooldown);
	}
}

FString UElectrocute::GetNextLevelDescription(int32 Level)
{
	const int ScaledDamage = Damage.GetValueAtLevel(Level);
	const float ManaCost = FMath::Abs(GetManaCost(Level));
	const float Cooldown = GetCooldown(Level);
	return FString::Printf(TEXT(
		"<Title>连锁闪电</>\n"
					
		"<Small>下一等级：</><Level>%d</>\n\n"
					
		"<Default>向指定方向发射闪电，在击中敌人后分额外分散</><Damage>%d</><Default>个子闪电，"
		"闪电造成</><Damage>%d</><Default>点雷电伤害</>有一定概率使敌人麻痹（无法行动）\n\n"
					
		"<Small>法力值消耗：</><ManaCost>%.1f</>\n"
		"<Small>冷却时间：</><Cooldown>%.1f</>\n"),
		Level,
		FMath::Min(Level-1,MaxNumShockTargets),
		ScaledDamage,
		ManaCost,
		Cooldown);
}
