

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffectExecutionCalculation.h"
#include "EC_Damage.generated.h"

/**
 * 
 */
UCLASS()
class MYAURA_API UEC_Damage : public UGameplayEffectExecutionCalculation
{
	GENERATED_BODY()
public:
	UEC_Damage();
	void DetermainDebuff(const FGameplayEffectCustomExecutionParameters& ExecutionParams,
	                     const FGameplayEffectSpec& Spec,
	                     FAggregatorEvaluateParameters EvaluateParameters,
	                     const TMap<FGameplayTag,FGameplayEffectAttributeCaptureDefinition>& TagsToCaptureDefs) const;

	virtual void Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams, FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const override;
};
