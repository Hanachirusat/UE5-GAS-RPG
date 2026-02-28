#include "AbilitySystem/MMC/MMC_MaxHP.h"

#include "AbilitySystem/AuraAttributeSet.h"
#include "Interaction/CombatInterface.h"

UMMC_MaxHP::UMMC_MaxHP()
{
	//静态函数，可以直接通过类名调用
	VigorDef.AttributeToCapture = UAuraAttributeSet::GetVigorAttribute();
	//定义从source还是target捕获
	VigorDef.AttributeSource = EGameplayEffectAttributeCaptureSource::Target;
	VigorDef.bSnapshot = false; //什么时候捕获？Spec创建的时候还是应用的时候？快照是spec创建的时候就
	RelevantAttributesToCapture.Add(VigorDef);
}

float UMMC_MaxHP::CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const
{
	// Gether tags from source and target
	const FGameplayTagContainer* SourceTags = Spec.CapturedSourceTags.GetAggregatedTags();
	const FGameplayTagContainer* TargetTags = Spec.CapturedTargetTags.GetAggregatedTags();

	//捕获属性并修改值
	FAggregatorEvaluateParameters EvaluateParameters;
	EvaluateParameters.SourceTags = SourceTags;
	EvaluateParameters.TargetTags = TargetTags;

	float CapVigor = 0.f;
	GetCapturedAttributeMagnitude(VigorDef, Spec, EvaluateParameters, CapVigor);
	CapVigor = FMath::Max<float>(CapVigor, 0.f);

	int32 Level = 1.f;
	if (Spec.GetContext().GetSourceObject()->Implements<UCombatInterface>())
	{
		Level = ICombatInterface::Execute_GetLevel(Spec.GetContext().GetSourceObject());
	}

	return 80.f + 2.5f * CapVigor + 10.f * Level;
}
