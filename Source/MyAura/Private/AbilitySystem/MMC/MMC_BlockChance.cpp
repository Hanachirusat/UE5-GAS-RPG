


#include "AbilitySystem/MMC/MMC_BlockChance.h"

#include "AbilitySystem/AuraAttributeSet.h"

UMMC_BlockChance::UMMC_BlockChance()
{
	//静态函数，可以直接通过类名调用
	ArmorDef.AttributeToCapture = UAuraAttributeSet::GetArmorAttribute();
	//定义从source还是target捕获
	ArmorDef.AttributeSource = EGameplayEffectAttributeCaptureSource::Target;
	ArmorDef.bSnapshot = false; //什么时候捕获？Spec创建的时候还是应用的时候？快照是spec创建的时候就
	RelevantAttributesToCapture.Add(ArmorDef);
}

float UMMC_BlockChance::CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const
{
	// Gether tags from source and target
	const FGameplayTagContainer* SourceTags = Spec.CapturedSourceTags.GetAggregatedTags();
	const FGameplayTagContainer* TargetTags = Spec.CapturedTargetTags.GetAggregatedTags();

	//捕获属性并修改值
	FAggregatorEvaluateParameters EvaluateParameters;
	EvaluateParameters.SourceTags = SourceTags;
	EvaluateParameters.TargetTags = TargetTags;

	float CapArmor = 0.f;
	GetCapturedAttributeMagnitude(ArmorDef, Spec, EvaluateParameters, CapArmor);
	
	CapArmor = FMath::Max<float>(CapArmor, 0.f);
	

	return 4.f + 0.25f * CapArmor;
}
