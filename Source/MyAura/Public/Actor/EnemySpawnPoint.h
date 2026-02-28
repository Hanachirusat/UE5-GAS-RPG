

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Data/CharacterClassInfo.h"
#include "Engine/TargetPoint.h"
#include "EnemySpawnPoint.generated.h"

class AEnemyCharacter;
/**
 * 
 */
UCLASS()
class MYAURA_API AEnemySpawnPoint : public ATargetPoint
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category = "Enemy Info")
	TSubclassOf<AEnemyCharacter> EnemyClass;
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category = "Enemy Info")
	int32 EnemyLevel = 1;
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category = "Enemy Info")
	ECharacterClass CharacterClass = ECharacterClass::Warrior;
	
	UFUNCTION(BlueprintCallable)
	void SpawnEnemy();
};
