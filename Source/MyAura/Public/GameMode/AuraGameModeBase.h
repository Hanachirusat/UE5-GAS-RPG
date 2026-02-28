

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "AuraGameModeBase.generated.h"

class ULootTiers;
class ULoadScreenSaveGame;
class USaveGame;
class UMVVM_LoadSlot;
class UAbilityInfo;
class UCharacterClassInfo;
/**
 * 
 */
UCLASS()
class MYAURA_API AAuraGameModeBase : public AGameModeBase
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditDefaultsOnly , Category = "Character Class Defaults")
	TObjectPtr<UCharacterClassInfo> CharacterClass;
	
	UPROPERTY(EditDefaultsOnly , Category = "Ability Info")
	TObjectPtr<UAbilityInfo> AbilityInfo;
	
	UPROPERTY(EditDefaultsOnly , Category = "LootTiers")
	TObjectPtr<ULootTiers> LootTiers;
	
	UPROPERTY(EditDefaultsOnly )
	TSubclassOf<USaveGame> LoadScreenSaveGameClass;
	
	UPROPERTY(EditDefaultsOnly )
	FString DefaultMapName;
	
	UPROPERTY(EditDefaultsOnly )
	TSoftObjectPtr<UWorld> DefaultMap;
	
	UPROPERTY(EditDefaultsOnly )
	TMap<FString,TSoftObjectPtr<UWorld>> Maps;
	
	UPROPERTY(EditDefaultsOnly )
	FName DefaultPlayerStartTag;
	
	
	static void DeleteSlot(const FString& SlotName, int32 SlotIndex);
	void SaveSlotDate(UMVVM_LoadSlot* LoadSlot,int32 SlotIndex);
	ULoadScreenSaveGame* GetSaveSlotDate(const FString& SlotName, int32 SlotIndex) const;
	void TravelToMap(UMVVM_LoadSlot* Slot);
	ULoadScreenSaveGame* GetLoadSlotDate() const;
	void SaveGameProgressDate(ULoadScreenSaveGame* SaveGame);
	void SaveWorldState(UWorld* World,const FString& InNextMapAssetName = FString("")) const;
	void LoadWorldState(UWorld* World) const;
	
	FString GetMapNameFromMapAssetName(const FString& MapAssetName) const;
	
	virtual AActor* ChoosePlayerStart_Implementation(AController* Player) override;
	
	void PlayerDied(ACharacter* Player);
protected:
	virtual void BeginPlay() override;
	
};
