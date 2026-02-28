


#include "AuraGameplayTags.h"
#include "GameplayTagsManager.h"

FAuraGameplayTags FAuraGameplayTags::GameplayTags;

void FAuraGameplayTags::InitializeNativeGameplayTags()
{
    // ====== 主要属性 ======
    GameplayTags.Attribute_Primary_Strength = UGameplayTagsManager::Get().AddNativeGameplayTag(
        FName("Attributes.Primary.Strength"),
        FString("Increases physical damage")
    );
    
    GameplayTags.Attribute_Primary_Intelligence = UGameplayTagsManager::Get().AddNativeGameplayTag(
        FName("Attributes.Primary.Intelligence"),
        FString("Increases magical damage")
    );
    
    GameplayTags.Attribute_Primary_Resilience = UGameplayTagsManager::Get().AddNativeGameplayTag(
        FName("Attributes.Primary.Resilience"),
        FString("Increases armor and armor Penetration")
    );
    
    GameplayTags.Attribute_Primary_Vigor = UGameplayTagsManager::Get().AddNativeGameplayTag(
        FName("Attributes.Primary.Vigor"),
        FString("Increases health")
    );
    
    // ====== 次级属性 ======
    GameplayTags.Attribute_Secondary_MaxHP = UGameplayTagsManager::Get().AddNativeGameplayTag(
        FName("Attributes.Secondary.MaxHP"),
        FString("Maximum health")
    );
    
    GameplayTags.Attribute_Secondary_MaxMana = UGameplayTagsManager::Get().AddNativeGameplayTag(
        FName("Attributes.Secondary.MaxMana"),
        FString("Maximum mana")
    );
    
    GameplayTags.Attribute_Secondary_Armor = UGameplayTagsManager::Get().AddNativeGameplayTag(
        FName("Attributes.Secondary.Armor"),
        FString("Reduces damage taken, improves Block Chance")
    );
    
    GameplayTags.Attribute_Secondary_ArmorPenetration = UGameplayTagsManager::Get().AddNativeGameplayTag(
        FName("Attributes.Secondary.ArmorPenetration"),
        FString("Ignores percentage of enemy armor, Increase Critical Hit Chance")
    );
    
    GameplayTags.Attribute_Secondary_BlockChance = UGameplayTagsManager::Get().AddNativeGameplayTag(
        FName("Attributes.Secondary.BlockChance"),
        FString("Chance to cut incoming damage in half")
    );
    
    GameplayTags.Attribute_Secondary_CriticalHitChance = UGameplayTagsManager::Get().AddNativeGameplayTag(
        FName("Attributes.Secondary.CriticalHitChance"),
        FString("Chance to double a damage plus critical hit bonus")
    );
    
    GameplayTags.Attribute_Secondary_CriticalHitDamage = UGameplayTagsManager::Get().AddNativeGameplayTag(
        FName("Attributes.Secondary.CriticalHitDamage"),
        FString("Bonus damage added when a critical hit is scored")
    );
    
    GameplayTags.Attribute_Secondary_CriticalHitResistance = UGameplayTagsManager::Get().AddNativeGameplayTag(
        FName("Attributes.Secondary.CriticalHitResistance"),
        FString("Reduces critical hits chance of attacking enemies")
    );
    
    GameplayTags.Attribute_Secondary_HealthRegeneration = UGameplayTagsManager::Get().AddNativeGameplayTag(
        FName("Attributes.Secondary.HealthRegeneration"),
        FString("Amount of health regenerated per second")
    );
    
    GameplayTags.Attribute_Secondary_ManaRegeneration = UGameplayTagsManager::Get().AddNativeGameplayTag(
        FName("Attributes.Secondary.ManaRegeneration"),
        FString("Amount of mana regenerated per second")
    );
	
    GameplayTags.Attribute_Meta_IncomingXP = UGameplayTagsManager::Get().AddNativeGameplayTag(
    FName("Attributes.Meta.IncomingXP"),
    FString("IncomingXP Tag")
    );
    
    
    
    
    //输入
    GameplayTags.InputTag_LMB = UGameplayTagsManager::Get().AddNativeGameplayTag(
    FName("InputTag.LMB"),
    FString("Input Tag for Left  Mouse Button")
    );
    
    GameplayTags.InputTag_RMB = UGameplayTagsManager::Get().AddNativeGameplayTag(
    FName("InputTag.RMB"),
    FString("Input Tag for Right  Mouse Button")
    );
    
    GameplayTags.InputTag_1 = UGameplayTagsManager::Get().AddNativeGameplayTag(
    FName("InputTag.1"),
    FString("Input Tag for 1 key")
    );
    
    GameplayTags.InputTag_2 = UGameplayTagsManager::Get().AddNativeGameplayTag(
    FName("InputTag.2"),
    FString("Input Tag for 2 key")
    );
    
    GameplayTags.InputTag_3 = UGameplayTagsManager::Get().AddNativeGameplayTag(
    FName("InputTag.3"),
    FString("Input Tag for 3 key")
    );
    
    GameplayTags.InputTag_4 = UGameplayTagsManager::Get().AddNativeGameplayTag(
    FName("InputTag.4"),
    FString("Input Tag for 4 key")
    );
    GameplayTags.InputTag_Passive1 = UGameplayTagsManager::Get().AddNativeGameplayTag(
    FName("InputTag.Passive1"),
    FString("Input Tag for 4 key")
    );
    GameplayTags.InputTag_Passive2 = UGameplayTagsManager::Get().AddNativeGameplayTag(
    FName("InputTag.Passive2"),
    FString("Input Tag for 4 key")
    );
    
    //GAS-Damage
    GameplayTags.Damage = UGameplayTagsManager::Get().AddNativeGameplayTag(
    FName("Damage"),
    FString("Damage")
    );
    
    GameplayTags.Damage_Fire = UGameplayTagsManager::Get().AddNativeGameplayTag(
    FName("Damage.Fire"),
    FString("Fire Damage Type")
    );
    
    GameplayTags.Damage_Lightning = UGameplayTagsManager::Get().AddNativeGameplayTag(
    FName("Damage.Lighting"),
    FString("Lighting Damage Type")
    );
    
    GameplayTags.Damage_Arcane = UGameplayTagsManager::Get().AddNativeGameplayTag(
    FName("Damage.Arcane"),
    FString("Arcane Damage Type")
    );
    
    GameplayTags.Damage_Physical = UGameplayTagsManager::Get().AddNativeGameplayTag(
    FName("Damage.Physical"),
    FString("Physical Damage Type")
    );
    
    GameplayTags.Debuff_Fire = UGameplayTagsManager::Get().AddNativeGameplayTag(
    FName("Debuff.Fire"),
    FString("Debuff Type")
    );
    GameplayTags.Debuff_Lightning = UGameplayTagsManager::Get().AddNativeGameplayTag(
    FName("Debuff.Lightning"),
    FString("Debuff Type")
    );
    GameplayTags.Debuff_Arcane = UGameplayTagsManager::Get().AddNativeGameplayTag(
    FName("Debuff.Arcane"),
    FString("Debuff Type")
    );
    GameplayTags.Debuff_Physical = UGameplayTagsManager::Get().AddNativeGameplayTag(
    FName("Debuff.Physical"),
    FString("Debuff Type")
    );
    GameplayTags.Debuff_Chance = UGameplayTagsManager::Get().AddNativeGameplayTag(
    FName("Debuff.Chance"),
    FString("Debuff Type")
    );
    GameplayTags.Debuff_Duration = UGameplayTagsManager::Get().AddNativeGameplayTag(
    FName("Debuff.Duration"),
    FString("Debuff Type")
    );
    GameplayTags.Debuff_Damage = UGameplayTagsManager::Get().AddNativeGameplayTag(
    FName("Debuff.Damage"),
    FString("Debuff Type")
    );
    GameplayTags.Debuff_Frequency = UGameplayTagsManager::Get().AddNativeGameplayTag(
    FName("Debuff.Frequency"),
    FString("Debuff Type")
    );
    
    

    GameplayTags.Attribute_Resistance_Fire = UGameplayTagsManager::Get().AddNativeGameplayTag(
    FName("Attributes.Resistance.Fire"),
    FString("Resistance to Fire Damage Type")
    );
    
    GameplayTags.Attribute_Resistance_Lightning = UGameplayTagsManager::Get().AddNativeGameplayTag(
    FName("Attributes.Resistance.Lightning"),
    FString("Resistance to Lightning Damage Type")
    );
    
    GameplayTags.Attribute_Resistance_Arcane = UGameplayTagsManager::Get().AddNativeGameplayTag(
    FName("Attributes.Resistance.Arcane"),
    FString("Resistance to Arcane Damage Type")
    );
    
    GameplayTags.Attribute_Resistance_Physical = UGameplayTagsManager::Get().AddNativeGameplayTag(
    FName("Attributes.Resistance.Physical"),
    FString("Resistance to Physical Damage Type")
    );
    
    
    
    //Map of Damage Types to Resistance
    GameplayTags.DamageTypesToResistances.Add(GameplayTags.Damage_Arcane,GameplayTags.Attribute_Resistance_Arcane);
    GameplayTags.DamageTypesToResistances.Add(GameplayTags.Damage_Fire,GameplayTags.Attribute_Resistance_Fire);
    GameplayTags.DamageTypesToResistances.Add(GameplayTags.Damage_Lightning,GameplayTags.Attribute_Resistance_Lightning);
    GameplayTags.DamageTypesToResistances.Add(GameplayTags.Damage_Physical,GameplayTags.Attribute_Resistance_Physical);
        
    //Map of Damage Types to Debuff Type
    GameplayTags.DamageTypesToDebuffs.Add(GameplayTags.Damage_Fire,GameplayTags.Debuff_Fire);
    GameplayTags.DamageTypesToDebuffs.Add(GameplayTags.Damage_Lightning,GameplayTags.Debuff_Lightning);
    GameplayTags.DamageTypesToDebuffs.Add(GameplayTags.Damage_Arcane,GameplayTags.Damage_Arcane);
    GameplayTags.DamageTypesToDebuffs.Add(GameplayTags.Damage_Physical,GameplayTags.Damage_Physical);
    
    
    //GAS-Common
    GameplayTags.Abilities_HitReact = UGameplayTagsManager::Get().AddNativeGameplayTag(
    FName("Abilities.HitReact"),
    FString("Tag Granted when Hit Reacting")
    );    
	
    ////////////////abilities
    GameplayTags.Abilities_Attack = UGameplayTagsManager::Get().AddNativeGameplayTag(
    FName("Abilities.Attack"),
    FString("Attack Ability Tag")
    );    
    GameplayTags.Abilities_Summon = UGameplayTagsManager::Get().AddNativeGameplayTag(
    FName("Abilities.Summon"),
    FString("Summon Ability Tag")
    );    
    GameplayTags.Abilities_None = UGameplayTagsManager::Get().AddNativeGameplayTag(
    FName("Abilities.None"),
    FString("None Ability Tag")
    );    
    
    GameplayTags.Abilities_Status_Eligible = UGameplayTagsManager::Get().AddNativeGameplayTag(
    FName("Abilities.Status.Eligible"),
    FString("")
    );    
    GameplayTags.Abilities_Status_Locked = UGameplayTagsManager::Get().AddNativeGameplayTag(
    FName("Abilities.Status.Locked"),
    FString("")
    );    
    GameplayTags.Abilities_Status_Unlocked = UGameplayTagsManager::Get().AddNativeGameplayTag(
    FName("Abilities.Status.Unlocked"),
    FString("")
    );    
    GameplayTags.Abilities_Status_Equipped = UGameplayTagsManager::Get().AddNativeGameplayTag(
    FName("Abilities.Status.Equipped"),
    FString("")
    );    
    GameplayTags.Abilities_Type_None = UGameplayTagsManager::Get().AddNativeGameplayTag(
    FName("Abilities.Type.None"),
    FString("")
    );    
    GameplayTags.Abilities_Type_Offensive = UGameplayTagsManager::Get().AddNativeGameplayTag(
    FName("Abilities.Type.Offensive"),
    FString("")
    );    
    GameplayTags.Abilities_Type_Passive = UGameplayTagsManager::Get().AddNativeGameplayTag(
    FName("Abilities.Type.Passive"),
    FString("")
    );    
    
    GameplayTags.Abilities_Fire_FireBolt = UGameplayTagsManager::Get().AddNativeGameplayTag(
    FName("Abilities.Fire.FireBolt"),
    FString("FireBolt Ability Tag")
    );    
    GameplayTags.Abilities_Fire_FireBlast = UGameplayTagsManager::Get().AddNativeGameplayTag(
    FName("Abilities.Fire.FireBlast"),
    FString("FireBlast Ability Tag")
    );    
    
    GameplayTags.Abilities_Lightning_Electrocute = UGameplayTagsManager::Get().AddNativeGameplayTag(
    FName("Abilities.Lightning.Electrocute"),
    FString("Electrocute Ability Tag")
    );    
    GameplayTags.Abilities_Arcane_Shards = UGameplayTagsManager::Get().AddNativeGameplayTag(
    FName("Abilities.Arcane.Shards"),
    FString("Arcane Shards Ability Tag")
    );    
    
    GameplayTags.Abilities_Passive_HaleOfProtection = UGameplayTagsManager::Get().AddNativeGameplayTag(
    FName("Abilities.Passive.HaloProtection"),
    FString("")
    );    
    GameplayTags.Abilities_Passive_LifeSiphon = UGameplayTagsManager::Get().AddNativeGameplayTag(
    FName("Abilities.Passive.LifeSiphon"),
    FString("")
    );    
    GameplayTags.Abilities_Passive_ManaSiphon = UGameplayTagsManager::Get().AddNativeGameplayTag(
    FName("Abilities.Passive.ManaSiphon"),
    FString("")
    );    
    
    
    
    GameplayTags.Cooldown_Fire_FireBolt = UGameplayTagsManager::Get().AddNativeGameplayTag(
    FName("Cooldown.Fire.FireBolt"),
    FString("Cooldown of FireBolt Ability Tag")
    );    
    
    //Montage
    GameplayTags.CombatSocket_Weapon = UGameplayTagsManager::Get().AddNativeGameplayTag(
    FName("CombatSocket.Weapon"),
    FString("Attack with Weapon Tag")
    );    

    GameplayTags.CombatSocket_LeftHand = UGameplayTagsManager::Get().AddNativeGameplayTag(
    FName("CombatSocket.LeftHand"),
    FString("Attack with Left Hand Tag")
    );    

    GameplayTags.CombatSocket_RightHand = UGameplayTagsManager::Get().AddNativeGameplayTag(
    FName("CombatSocket.RightHand"),
    FString("Attack With Right Hand Tag")
    );    
    
    GameplayTags.CombatSocket_Tail = UGameplayTagsManager::Get().AddNativeGameplayTag(
    FName("CombatSocket.Tail"),
    FString("Attack With Tail  Tag")
    );    
        
    
    GameplayTags.Montage_Attack_1 = UGameplayTagsManager::Get().AddNativeGameplayTag(
    FName("Montage.Attack.1"),
    FString("Montage Attack 1")
    );    
    
    GameplayTags.Montage_Attack_2 = UGameplayTagsManager::Get().AddNativeGameplayTag(
    FName("Montage.Attack.2"),
    FString("Montage Attack 2")
    );    
    
    GameplayTags.Montage_Attack_3 = UGameplayTagsManager::Get().AddNativeGameplayTag(
    FName("Montage.Attack.3"),
    FString("Montage Attack 3")
    );    
    
    GameplayTags.Montage_Attack_4 = UGameplayTagsManager::Get().AddNativeGameplayTag(
    FName("Montage.Attack.4"),
    FString("Montage Attack 4")
    );    
    
    GameplayTags.Player_Block_InputPressed = UGameplayTagsManager::Get().AddNativeGameplayTag(
    FName("Player.Block.InputPressed"),
    FString("Player.Block.InputPressed")
    );    
    
    GameplayTags.Player_Block_InputHeld = UGameplayTagsManager::Get().AddNativeGameplayTag(
    FName("Player.Block.InputHeld"),
    FString("Player.Block.InputPressed")
    );    
    
    GameplayTags.Player_Block_InputReleased = UGameplayTagsManager::Get().AddNativeGameplayTag(
    FName("Player.Block.InputReleased"),
    FString("Player.Block.InputPressed")
    );    
    
    GameplayTags.Player_Block_CursorTrace = UGameplayTagsManager::Get().AddNativeGameplayTag(
    FName("Player.Block.CursorTrace"),
    FString("Player.Block.InputPressed")
    );    
    
    GameplayTags.GameplayCue_FireBlast = UGameplayTagsManager::Get().AddNativeGameplayTag(
    FName("GameplayCue.FireBlast"),
    FString("")
    );    
    
}


