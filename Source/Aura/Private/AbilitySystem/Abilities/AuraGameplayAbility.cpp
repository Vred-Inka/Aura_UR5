// Aura by Vredinka


#include "AbilitySystem/Abilities/AuraGameplayAbility.h"

FString UAuraGameplayAbility::GetDescription(int32 Level)
{
	return FString::Printf(TEXT("<Default>%s</><Level>%d</>"), L"Default Ability Name - SomeText SomeText SomeText SomeText SomeText SomeText SomeText SomeText SomeText SomeText SomeText SomeText", Level);
}

FString UAuraGameplayAbility::GetNextLevelDescription(int32 Level)
{
	return FString::Printf(TEXT("<Default>Next Level: </><Level>%d</> \n <Default>Causes much more damage </>"), Level);
}

FString UAuraGameplayAbility::GetLockerDescription(int32 Level)
{
	return FString::Printf(TEXT("<Default> Spell Locked Until Level: %d</>"), Level);
}
