

#pragma once

#include "CoreMinimal.h"
#include "AuraInputConfig.h"
#include "EnhancedInputComponent.h"
#include "AuraEnhancedInputComponent.generated.h"

/**
 * 
 */
UCLASS()
class MYAURA_API UAuraEnhancedInputComponent : public UEnhancedInputComponent
{
	GENERATED_BODY()
public:
	
	template<class UserClass, typename PressedFuncType,typename ReleaseFuncType, typename HeldFuncType>
	void BindAbilityActions(const UAuraInputConfig* InputConfig,UserClass* Object,PressedFuncType PressedFunc, ReleaseFuncType ReleaseFunc, HeldFuncType HeldFunc);
};

template <class UserClass, typename PressedFuncType, typename ReleaseFuncType, typename HeldFuncType>
void UAuraEnhancedInputComponent::BindAbilityActions(const UAuraInputConfig* InputConfig, UserClass* Object,
	PressedFuncType PressedFunc, ReleaseFuncType ReleaseFunc, HeldFuncType HeldFunc)
{
	check(InputConfig);
	
	for (const FAuraInputAction& InputAction : InputConfig->AbilityInputActions)
	{
		if (InputAction.InputAction && InputAction.InputTag.IsValid())
		{
			if (PressedFunc)
			{
				//started在被按下的一瞬间调用一次
				BindAction(InputAction.InputAction,ETriggerEvent::Started,Object,PressedFunc, InputAction.InputTag);
			}
			if (ReleaseFunc)
			{
				// completed在松开按键的一瞬间调用一次
				BindAction(InputAction.InputAction,ETriggerEvent::Completed,Object,ReleaseFunc,InputAction.InputTag);
			}
			if (HeldFunc)
			{
				//Triggered确保只要按下，每一tick都调用这个函数,最后一个参数会被传给HeldFunc
				BindAction(InputAction.InputAction,ETriggerEvent::Triggered,Object,HeldFunc,InputAction.InputTag);
			}
		}
	}
	
}
