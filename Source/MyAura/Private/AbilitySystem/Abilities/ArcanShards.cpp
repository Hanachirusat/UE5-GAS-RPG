


#include "AbilitySystem/Abilities/ArcanShards.h"

FString UArcanShards::GetDescription(int32 Level)
{
	const int ScaledDamage = Damage.GetValueAtLevel(Level);
	const float ManaCost = FMath::Abs(GetManaCost(Level));
	const float Cooldown = GetCooldown(Level);
	if (Level ==1)
	{
		return FString::Printf(TEXT(
			"<Title>爆炸水晶</>\n"
			
			"<Small>等级：</><Level>%d</>\n\n"
			
			"<Default>在指定区域内生成</><Damage>1</><Default>个水晶，"
			"水晶造成</><Damage>%d</><Default>点奥术伤害</>\n\n"
			
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
			"<Title>爆炸水晶</>\n"
					
			"<Small>等级：</><Level>%d</>\n\n"
					
			"<Default>在指定区域内生成</><Damage>%d</><Default>个水晶，"
			"水晶造成</><Damage>%d</><Default>点奥术伤害</>\n\n"
			
			"<Small>法力值消耗：</><ManaCost>%.1f</>\n"
			"<Small>冷却时间：</><Cooldown>%.1f</>\n"),
			Level,
			FMath::Min(Level,MaxNumShards),
			ScaledDamage,
			ManaCost,
			Cooldown);
	}
}

FString UArcanShards::GetNextLevelDescription(int32 Level)
{
	const int ScaledDamage = Damage.GetValueAtLevel(Level);
	const float ManaCost = FMath::Abs(GetManaCost(Level));
	const float Cooldown = GetCooldown(Level);
	return FString::Printf(TEXT(
	"<Title>爆炸水晶</>\n"
					
	"<Small>下一等级：</><Level>%d</>\n\n"
					
	"<Default>在指定区域内生成</><Damage>%d</><Default>个水晶，"
	"水晶造成</><Damage>%d</><Default>点奥术伤害</>\n\n"
			
	"<Small>法力值消耗：</><ManaCost>%.1f</>\n"
	"<Small>冷却时间：</><Cooldown>%.1f</>\n"),
	Level,
	FMath::Min(Level,MaxNumShards),
	ScaledDamage,
	ManaCost,
	Cooldown);
}
