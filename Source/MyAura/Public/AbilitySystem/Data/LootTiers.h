

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "LootTiers.generated.h"

USTRUCT(BlueprintType)
struct FLootItem
{
	GENERATED_BODY()
	
	UPROPERTY(BlueprintReadOnly, EditAnywhere,Category = "LoopTiers")
	TSubclassOf<AActor> LootClass;
	
	UPROPERTY( EditAnywhere,Category = "LoopTiers")
	float ChanceToSpawn = 0.f;
	
	UPROPERTY( EditAnywhere,Category = "LoopTiers")
	int32 MaxNumToSpawn = 0.f;
	
	UPROPERTY(BlueprintReadOnly, EditAnywhere,Category = "LoopTiers")
	bool bLootLevelOverride = true;
	
	
};
UCLASS()
class MYAURA_API ULootTiers : public UDataAsset
{
	GENERATED_BODY()
	
public:
	
	UPROPERTY(EditAnywhere,Category = "LoopTiers")
	TArray<FLootItem> LootItems;
	
	UFUNCTION(BlueprintCallable)
	TArray<FLootItem> GetLootItems();  //每一个Item可能会有多个副本
	
};
