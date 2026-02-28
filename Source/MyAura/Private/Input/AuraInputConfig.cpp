


#include "Input/AuraInputConfig.h"

const UInputAction* UAuraInputConfig::FindAbilityInputActionByTag(const FGameplayTag InputTag, bool bLogNotFound) const
{
	for (const FAuraInputAction& InputAction : AbilityInputActions)
	{
		if (InputAction.InputAction && InputAction.InputTag == InputTag)
		{
			return InputAction.InputAction;
		}
	}
	if (bLogNotFound)
	{
		UE_LOG(LogTemp, Error,TEXT("Can't find AbilityInputAction For InputTag [%s], on InputConfig [%s]"),*InputTag.ToString(),*GetNameSafe(this));
	}
	return nullptr;
}
