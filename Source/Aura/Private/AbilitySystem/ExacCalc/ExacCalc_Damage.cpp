// Aura by Vredinka


#include "AbilitySystem/ExacCalc/ExacCalc_Damage.h"
#include "AbilitySystemComponent.h"

UExacCalc_Damage::UExacCalc_Damage()
{
	
}

void UExacCalc_Damage::Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams,
	FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const
{
	const UAbilitySystemComponent* SourceASC = ExecutionParams.GetSourceAbilitySystemComponent();
	const UAbilitySystemComponent* TargetASC = ExecutionParams.GetTargetAbilitySystemComponent();

	const AActor* SourceAvetar = SourceASC ?  SourceASC->GetAvatarActor() : nullptr;
	const AActor* TargetAvetar = TargetASC ?  TargetASC->GetAvatarActor() : nullptr;

	const FGameplayEffectSpec& Spec = ExecutionParams.GetOwningSpec();
}
