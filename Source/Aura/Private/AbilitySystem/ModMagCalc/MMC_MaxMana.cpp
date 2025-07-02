// Aura by Vredinka


#include "AbilitySystem/ModMagCalc/MMC_MaxMana.h"

#include "AbilitySystem/AuraAttributeSet.h"
#include "Interaction/CombatInterface.h"

UMMC_MaxMana::UMMC_MaxMana()
{
	IntelagenceDef.AttributeToCapture = UAuraAttributeSet::GetIntelligenceAttribute();
	IntelagenceDef.AttributeSource = EGameplayEffectAttributeCaptureSource::Target;
	IntelagenceDef.bSnapshot = false;

	RelevantAttributesToCapture.Add(IntelagenceDef);
}

float UMMC_MaxMana::CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const
{
	const FGameplayTagContainer* SourceTags = Spec.CapturedSourceTags.GetAggregatedTags();
	const FGameplayTagContainer* TargetTags = Spec.CapturedTargetTags.GetAggregatedTags();

	FAggregatorEvaluateParameters EvaluationParameters;
	EvaluationParameters.SourceTags = SourceTags;
	EvaluationParameters.TargetTags = TargetTags;

	float Intellegance = 0.0f;
	GetCapturedAttributeMagnitude(IntelagenceDef, Spec, EvaluationParameters, Intellegance);

	Intellegance = FMath::Max<float>(Intellegance, 0.0f);


	ICombatInterface* CombatInterface = Cast<ICombatInterface>(Spec.GetContext().GetSourceObject());

	const int32 PlayerLevel =  CombatInterface->GetPlayerLevel();
	
	return 50.f + Intellegance * 2.0f + 15.f * PlayerLevel;
}
