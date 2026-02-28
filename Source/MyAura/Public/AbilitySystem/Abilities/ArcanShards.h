

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/AuraDamageGameplayAbility.h"
#include "ArcanShards.generated.h"

/**
 * 
 */
UCLASS()
class MYAURA_API UArcanShards : public UAuraDamageGameplayAbility
{
	GENERATED_BODY()
public:
	virtual FString GetDescription(int32 Level) override;
	virtual FString GetNextLevelDescription(int32 Level) override;
	
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly)
	int32 MaxNumShards = 6;
};
