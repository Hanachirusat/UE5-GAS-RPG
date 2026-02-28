


#include "AbilitySystem/MMC/MMC_MaxMana.h"

#include "AbilitySystem/AuraAttributeSet.h"
#include "Interaction/CombatInterface.h"


UMMC_MaxMana::UMMC_MaxMana()
{
	//静态函数，可以直接通过类名调用
	IntelligenceDef.AttributeToCapture = UAuraAttributeSet::GetIntelligenceAttribute();
	//定义从source还是target捕获
	IntelligenceDef.AttributeSource = EGameplayEffectAttributeCaptureSource::Target;
	IntelligenceDef.bSnapshot = false; //什么时候捕获？Spec创建的时候还是应用的时候？快照是spec创建的时候就
	RelevantAttributesToCapture.Add(IntelligenceDef);
}


float UMMC_MaxMana::CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const
{
	// Gether tags from source and target
	const FGameplayTagContainer* SourceTags = Spec.CapturedSourceTags.GetAggregatedTags();
	const FGameplayTagContainer* TargetTags = Spec.CapturedTargetTags.GetAggregatedTags();

	//捕获属性并修改值
	FAggregatorEvaluateParameters EvaluateParameters;
	EvaluateParameters.SourceTags = SourceTags;
	EvaluateParameters.TargetTags = TargetTags;

	float CapIntelligence = 0.f;
	GetCapturedAttributeMagnitude(IntelligenceDef, Spec, EvaluateParameters, CapIntelligence);
	CapIntelligence = FMath::Max<float>(CapIntelligence, 0.f);

	int32 Level = 1.f;
	if (Spec.GetContext().GetSourceObject()->Implements<UCombatInterface>())
	{
		Level = ICombatInterface::Execute_GetLevel(Spec.GetContext().GetSourceObject());
	}

	return 50.f + 2.5f * CapIntelligence + 15.f * Level;
}
