// Aura by Vredinka


#include "UI/WidgetController/SpellMenuWidgetController.h"

#include "AuraGameplayTags.h"
#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "AbilitySystem/Data/AbilityInfo.h"
#include "Player/AuraPlayerState.h"

void USpellMenuWidgetController::BroadcastInitialValues()
{
	BroadcastAbilityInfo();
	SpellPointsChanged.Broadcast(GetAuraPS()->GetSpellPoints());
}

void USpellMenuWidgetController::BindCallbacksToDependencies()
{
	GetAuraASC()->AbilityStatusChangedDelegate.AddLambda([this](const FGameplayTag& AbilityTag, const FGameplayTag& StatusTag, int32 NewLevel)
	{
		if (SelectedAbility.AbilityTag.MatchesTagExact(AbilityTag))
		{
			SelectedAbility.StatusTag = StatusTag;
			bool bEnableSpendPoints =  false;
			bool bEnableEquip = false;
			ShouldEnableButtons(StatusTag, CurrentSpellPoints, bEnableSpendPoints, bEnableEquip);
			FString DescriptionString;
			FString NextLevelDescriptionString;
			GetAuraASC()->GetDescriptionsByAbilityTag(AbilityTag, DescriptionString, NextLevelDescriptionString);
			SpellGlobeSelectedDelegate.Broadcast(bEnableSpendPoints, bEnableEquip, DescriptionString, NextLevelDescriptionString);
		}
		
		if (AbilityInfo)
		{
			FAuraAbilityInfo Info = AbilityInfo->FindAbilityInfoForTag(AbilityTag);
			Info.StatusTag = StatusTag;
			AbilityInfoDelegate.Broadcast(Info);
		}
	});

	GetAuraPS()->OnSpellPointsChangedDelegate.AddLambda([this](const int32 SpellPoints)
	{
		SpellPointsChanged.Broadcast(SpellPoints);
		CurrentSpellPoints = SpellPoints;

		bool bEnableSpendPoints =  false;
		bool bEnableEquip = false;
		ShouldEnableButtons(SelectedAbility.StatusTag, CurrentSpellPoints, bEnableSpendPoints, bEnableEquip);
		FString DescriptionString;
		FString NextLevelDescriptionString;
		GetAuraASC()->GetDescriptionsByAbilityTag(SelectedAbility.AbilityTag, DescriptionString, NextLevelDescriptionString);
		SpellGlobeSelectedDelegate.Broadcast(bEnableSpendPoints, bEnableEquip, DescriptionString, NextLevelDescriptionString);
	});
	
}

void USpellMenuWidgetController::SpellGlobeSelected(const FGameplayTag& AbilityTag)
{
	const FAuraGameplayTags GameplayTags = FAuraGameplayTags::Get();
	const int32 SpellPoints = GetAuraPS()->GetSpellPoints();
	FGameplayTag AbilityStatus;

	bool bTagIsValid =  AbilityTag.IsValid();
	bool bTagIsNone = AbilityTag.MatchesTag(GameplayTags.Abilities_None);

	const FGameplayAbilitySpec* AbilitySpec = GetAuraASC()->GetSpecFromAbilityTag(AbilityTag);
	bool bSpecValid = AbilitySpec != nullptr;

	if (!bTagIsValid || bTagIsNone || !bSpecValid)
	{
		AbilityStatus = GameplayTags.Abilities_Status_Locked;
	}
	else
	{
		AbilityStatus =  GetAuraASC()->GetStatusTagFromSpec(*AbilitySpec);
	}

	SelectedAbility.AbilityTag = AbilityTag;
	SelectedAbility.StatusTag = AbilityStatus;

	bool bEnableSpendPoints =  false;
	bool bEnableEquip = false;
	ShouldEnableButtons(AbilityStatus, SpellPoints, bEnableSpendPoints, bEnableEquip);
	ShouldEnableButtons(SelectedAbility.StatusTag, CurrentSpellPoints, bEnableSpendPoints, bEnableEquip);
	FString DescriptionString;
	FString NextLevelDescriptionString;
	GetAuraASC()->GetDescriptionsByAbilityTag(SelectedAbility.AbilityTag, DescriptionString, NextLevelDescriptionString);
	SpellGlobeSelectedDelegate.Broadcast(bEnableSpendPoints, bEnableEquip, DescriptionString, NextLevelDescriptionString);
}

void USpellMenuWidgetController::SpendPointButtonPressed()
{
	if (GetAuraASC())
	{
		GetAuraASC()->ServerSpendSpellPoint(SelectedAbility.AbilityTag);
	}
}

void USpellMenuWidgetController::GlobeDeselect()
{
	SelectedAbility.AbilityTag =  FAuraGameplayTags::Get().Abilities_None;
	SelectedAbility.StatusTag =  FAuraGameplayTags::Get().Abilities_Status_Locked;
	
	SpellGlobeSelectedDelegate.Broadcast(false, false, FString(), FString());
}

void USpellMenuWidgetController::ShouldEnableButtons(const FGameplayTag& AbilityStatus, int32 SpellPoints,
                                                     bool& bSpendPointsButtonEnabled, bool& bEquipButtonEnabled)
{
	const FAuraGameplayTags GameplayTags = FAuraGameplayTags::Get();
	bSpendPointsButtonEnabled = false;
	bEquipButtonEnabled = false;

	if (AbilityStatus.MatchesTag(GameplayTags.Abilities_Status_Equipped))
	{
		bSpendPointsButtonEnabled =  SpellPoints > 0;
		bEquipButtonEnabled = true;
	}
	else if (AbilityStatus.MatchesTag(GameplayTags.Abilities_Status_Eligible))
	{
		bSpendPointsButtonEnabled =  SpellPoints > 0;
	}
	else if (AbilityStatus.MatchesTag(GameplayTags.Abilities_Status_Unlocked))
	{
		bSpendPointsButtonEnabled = SpellPoints > 0;
		bEquipButtonEnabled = true;
	}
}
