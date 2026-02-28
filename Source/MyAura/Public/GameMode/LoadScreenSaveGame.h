

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "GameFramework/SaveGame.h"
#include "LoadScreenSaveGame.generated.h"


class UGameplayAbility;

UENUM(Blueprintable)
enum ESaveSlotStatus
{
	Vacant,
	EnterName,
	Taken
};

USTRUCT()
struct FSavedActor
{
	GENERATED_BODY()
	
	UPROPERTY()
	FName ActorName = FName();
	
	UPROPERTY()
	FTransform Transform = FTransform();
	
	// Seriolized Varibles from the actor - obly these marked savegame specifier
	UPROPERTY()
	TArray<uint8> Bytes;
	
};

inline bool operator==(const FSavedActor& a, const FSavedActor& b)
{
	return a.ActorName == b.ActorName;
}

USTRUCT()
struct FSavedMap
{
	GENERATED_BODY()
	
	UPROPERTY()
	FString MapAssetName = FString();
	
	UPROPERTY()
	TArray<FSavedActor> SavedActors;
};


USTRUCT(BlueprintType)
struct FSavedAbility
{
	GENERATED_BODY()
	
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly,Category = "ClassDefaults")
	TSubclassOf<UGameplayAbility> GA;
	
	UPROPERTY(EditDefaultsOnly,BlueprintReadWrite,Category = "ClassDefaults")
	FGameplayTag AbilityTag = FGameplayTag();
	
		
	UPROPERTY(EditDefaultsOnly,BlueprintReadWrite,Category = "ClassDefaults")
	FGameplayTag AbilityStatus = FGameplayTag();
		
	UPROPERTY(EditDefaultsOnly,BlueprintReadWrite,Category = "ClassDefaults")
	FGameplayTag AbilitySlot = FGameplayTag();
	
	UPROPERTY(EditDefaultsOnly,BlueprintReadWrite,Category = "ClassDefaults")
	FGameplayTag AbilityType = FGameplayTag();
	
	UPROPERTY(EditDefaultsOnly,BlueprintReadWrite,Category = "ClassDefaults")
	int32 AbilityLevel = 1;
	
};

inline bool operator==(const FSavedAbility& a, const FSavedAbility& b)
{
	return a.AbilityTag.MatchesTagExact(b.AbilityTag);
}

UCLASS()
class MYAURA_API ULoadScreenSaveGame : public USaveGame
{
	GENERATED_BODY()
	
public:
	
	UPROPERTY()
	FString SlotName = FString();
	
	UPROPERTY()
	int32 SlotIndex = 0;
	
	UPROPERTY()
	FString PlayerName = FString("Default Name");
	
	UPROPERTY()
	FString MapName = FString("Default Name");
	
	UPROPERTY()
	FString MapAssetName = FString("Default Map Asset Name");
	
	UPROPERTY()
	FName PlayerStartTag = FName();
	
	UPROPERTY()
	TEnumAsByte<ESaveSlotStatus> SlotStatus = Vacant;
	
	UPROPERTY()
	bool bNewGame = true;
	/* Player */
	
	UPROPERTY()
	int32 PlayerLevel = 1;
	
	UPROPERTY()
	int32 XP = 0;
	
	UPROPERTY()
	int32 SpellPoints = 0;
	
	UPROPERTY()
	int32 AttributePoints = 0;
	
	UPROPERTY()
	float Strength = 0;
	
	UPROPERTY()
	float Intelligence = 0;
	
	UPROPERTY()
	float Resilience = 0;
	
	UPROPERTY()
	float Vigor = 0;
	
	UPROPERTY()
	TArray<FSavedMap> SavedMaps;
	
	/* Ability */
	UPROPERTY()
	TArray<FSavedAbility> SavedAbilities;
	
	FSavedMap GetSavedMapWithMapName(const FString& InMapName);
	bool HasMap(const FString& InMapName);
	
};
