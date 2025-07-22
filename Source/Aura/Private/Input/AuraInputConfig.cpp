// Aura by Vredinka


#include "Input/AuraInputConfig.h"

const UInputAction* UAuraInputConfig::GetInputActionForTag(const FGameplayTag& InputTag, bool bLogNotFound) const
{
	for (const FAuraInputAction& Action : AbilityInputActions)
	{
		if (Action.InputAction && Action.InputTag == InputTag)
		{
			return Action.InputAction;
		}		
	}

	if (bLogNotFound)
	{
		UE_LOG(LogTemp, Error, TEXT("InputAction not found in InputConfig [%s] for tag: %s"), *GetNameSafe(this),
			*InputTag.ToString());
	}
	
	return nullptr;
	
}
