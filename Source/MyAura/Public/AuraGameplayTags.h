

#pragma once

#include "CoreMinimal.h"
#include"GameplayTagContainer.h"

/**
 * AuraGameplayTags
 * 
 * Singleton 
 */

struct FAuraGameplayTags
{
public:
	static const FAuraGameplayTags& Get(){ return GameplayTags;}
	static void InitializeNativeGameplayTags();
	

	// 基础属性 - 主要
	FGameplayTag Attribute_Primary_Strength;
	FGameplayTag Attribute_Primary_Intelligence;
	FGameplayTag Attribute_Primary_Resilience;
	FGameplayTag Attribute_Primary_Vigor;
    
	// 次级属性
	FGameplayTag Attribute_Secondary_MaxHP;
	FGameplayTag Attribute_Secondary_MaxMana;
	FGameplayTag Attribute_Secondary_Armor;
	FGameplayTag Attribute_Secondary_ArmorPenetration;
	FGameplayTag Attribute_Secondary_BlockChance;
	FGameplayTag Attribute_Secondary_CriticalHitChance;
	FGameplayTag Attribute_Secondary_CriticalHitDamage;
	FGameplayTag Attribute_Secondary_CriticalHitResistance;
	FGameplayTag Attribute_Secondary_HealthRegeneration;
	FGameplayTag Attribute_Secondary_ManaRegeneration;
	
	FGameplayTag Attribute_Resistance_Fire;
	FGameplayTag Attribute_Resistance_Lightning;
	FGameplayTag Attribute_Resistance_Arcane;
	FGameplayTag Attribute_Resistance_Physical;
	
	FGameplayTag Attribute_Meta_IncomingXP;
	//输入
	FGameplayTag InputTag_LMB;
	FGameplayTag InputTag_RMB;
	FGameplayTag InputTag_1;
	FGameplayTag InputTag_2;
	FGameplayTag InputTag_3;
	FGameplayTag InputTag_4;
	FGameplayTag InputTag_Passive1;
	FGameplayTag InputTag_Passive2;

	//Ability
	FGameplayTag Damage;
	FGameplayTag Damage_Fire;
	FGameplayTag Damage_Lightning;//闪电
	FGameplayTag Damage_Arcane; //奥术
	FGameplayTag Damage_Physical; //物理
	
	//Debuff
	FGameplayTag Debuff_Fire;
	FGameplayTag Debuff_Lightning;
	FGameplayTag Debuff_Arcane;
	FGameplayTag Debuff_Physical;
	FGameplayTag Debuff_Chance;
	FGameplayTag Debuff_Damage;
	FGameplayTag Debuff_Duration;
	FGameplayTag Debuff_Frequency;
	
	FGameplayTag Abilities_Status_Locked;
	FGameplayTag Abilities_Status_Eligible;
	FGameplayTag Abilities_Status_Unlocked;
	FGameplayTag Abilities_Status_Equipped;
	
	FGameplayTag Abilities_Type_Offensive;
	FGameplayTag Abilities_Type_Passive;
	FGameplayTag Abilities_Type_None;
	
	FGameplayTag Abilities_Attack;
	FGameplayTag Abilities_Summon;
	FGameplayTag Abilities_HitReact;
	FGameplayTag Abilities_None;
	
	FGameplayTag Abilities_Fire_FireBolt;
	FGameplayTag Abilities_Fire_FireBlast;
	FGameplayTag Abilities_Lightning_Electrocute;
	FGameplayTag Abilities_Arcane_Shards;
	
	FGameplayTag Abilities_Passive_HaleOfProtection;
	FGameplayTag Abilities_Passive_LifeSiphon;
	FGameplayTag Abilities_Passive_ManaSiphon;
	
	FGameplayTag Cooldown_Fire_FireBolt;
	// FGameplayTag Cooldown_Lightning_FireBolt;
	//做出攻击动作的是哪个手还是武器？
	FGameplayTag CombatSocket_Weapon;
	FGameplayTag CombatSocket_RightHand;
	FGameplayTag CombatSocket_LeftHand;
	FGameplayTag CombatSocket_Tail;
	//每一个攻击蒙太奇对应不同的击中音效
	FGameplayTag Montage_Attack_1;
	FGameplayTag Montage_Attack_2;
	FGameplayTag Montage_Attack_3;
	FGameplayTag Montage_Attack_4;
	
	
	TMap<FGameplayTag,FGameplayTag> DamageTypesToResistances;
	TMap<FGameplayTag,FGameplayTag> DamageTypesToDebuffs;
	
	FGameplayTag Player_Block_InputPressed;
	FGameplayTag Player_Block_InputHeld;
	FGameplayTag Player_Block_InputReleased;
	FGameplayTag Player_Block_CursorTrace;
	
	FGameplayTag GameplayCue_FireBlast;
	
private:
	static FAuraGameplayTags GameplayTags;
};