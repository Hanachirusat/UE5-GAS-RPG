

#pragma once

#include "CoreMinimal.h"
#include "GameplayModMagnitudeCalculation.h"
#include "MMC_MaxHP.generated.h"

/**
 * 
 */
UCLASS()
class MYAURA_API UMMC_MaxHP : public UGameplayModMagnitudeCalculation
{
	GENERATED_BODY()
	
public:
	UMMC_MaxHP();
	
	virtual float CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const override;
	
private:
	
	FGameplayEffectAttributeCaptureDefinition VigorDef;  //Defination
};
