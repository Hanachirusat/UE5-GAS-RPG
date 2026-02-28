


#include "Actor/EnemySpawnPoint.h"

#include "Character/EnemyCharacter.h"
#include "GeometryCollection/GeometryCollectionComponent.h"

void AEnemySpawnPoint::SpawnEnemy()
{
	FActorSpawnParameters SpawnParams;
	//会自动调整位置
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
	AEnemyCharacter* Enemy = GetWorld()->SpawnActorDeferred<AEnemyCharacter>(EnemyClass,GetActorTransform());
	Enemy->SetLevel(EnemyLevel);
	Enemy->SetCharacterClass(CharacterClass);
	Enemy->FinishSpawning(GetActorTransform());
	Enemy->SpawnDefaultController(); //AI角色需要手动生成DefualtController
	
	
}
