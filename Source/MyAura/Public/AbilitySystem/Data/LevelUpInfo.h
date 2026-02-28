

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "LevelUpInfo.generated.h"

USTRUCT(BlueprintType)
struct FAuraLevelUpInfo
{
	GENERATED_BODY()
	
	UPROPERTY(EditDefaultsOnly)
	int32 LevelUpRequirement = 0;  //升到当前等级一共需要多少经验
	
	UPROPERTY(EditDefaultsOnly)
	int32 AttributePointAward = 1;
	
	UPROPERTY(EditDefaultsOnly)
	int32 SpellPointAward = 1;
	
	
	
};
UCLASS()
class MYAURA_API ULevelUpInfo : public UDataAsset
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditDefaultsOnly)
	TArray<FAuraLevelUpInfo> LevelUpInformation; 
	
	int32 FindLevelForXP(int32 XP) const;  
};
