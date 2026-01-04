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

	GetAuraASC()->AbilityEquippedDelegate.AddUObject(this, &USpellMenuWidgetController::OnAbilityEquipped);
	
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
	if (bWaitingForEquipSelection)
	{
		FGameplayTag SelectedAbilityType= AbilityInfo->FindAbilityInfoForTag(AbilityTag).AbilityType;
		StopWaitingForEquipDelegate.Broadcast(SelectedAbilityType);
		bWaitingForEquipSelection = false;
	}
	
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
	//ShouldEnableButtons(SelectedAbility.StatusTag, CurrentSpellPoints, bEnableSpendPoints, bEnableEquip);
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
	if (bWaitingForEquipSelection)
	{
		FGameplayTag SelectedAbilityType= AbilityInfo->FindAbilityInfoForTag(SelectedAbility.AbilityTag).AbilityType;
		StopWaitingForEquipDelegate.Broadcast(SelectedAbilityType);
		bWaitingForEquipSelection = false;
	}
	
	SelectedAbility.AbilityTag =  FAuraGameplayTags::Get().Abilities_None;
	SelectedAbility.StatusTag =  FAuraGameplayTags::Get().Abilities_Status_Locked;
	
	SpellGlobeSelectedDelegate.Broadcast(false, false, FString(), FString());
}

void USpellMenuWidgetController::EquipButtonPressed()
{
	const FGameplayTag AbilityType = AbilityInfo->FindAbilityInfoForTag(SelectedAbility.AbilityTag).AbilityType;
	WaitForEquipDelegate.Broadcast(AbilityType);
	bWaitingForEquipSelection = true;

	const FGameplayTag SelectedStatus =  GetAuraASC()->GetStatusTagFromAbilityTag(SelectedAbility.AbilityTag);
	if (SelectedStatus.MatchesTagExact(FAuraGameplayTags::Get().Abilities_Status_Equipped))
	{
		SelectedSlot = GetAuraASC()->GetSlotFromAbilityTag(SelectedAbility.AbilityTag);
	}
}

void USpellMenuWidgetController::SpellRowGlobePressed(const FGameplayTag& SlotTag, const FGameplayTag& AbilityType)
{
	if (!bWaitingForEquipSelection)
		return;

	//chack selected ability against the slot's ability type
	// (don't equip an offensive ability on a passive slot and vice versa)
	const FGameplayTag SelectedAbilityType = AbilityInfo->FindAbilityInfoForTag(SelectedAbility.AbilityTag).AbilityType;
	if (!SelectedAbilityType.MatchesTagExact(AbilityType))
		return;

	GetAuraASC()->ServerEquipAbility(SelectedAbility.AbilityTag, SlotTag);
}

void USpellMenuWidgetController::OnAbilityEquipped(const FGameplayTag& AbilityTag, const FGameplayTag& Status,
	const FGameplayTag& SlotTag, const FGameplayTag& PreviousSlotTag)
{
	bWaitingForEquipSelection = false;

	const FAuraGameplayTags& GameplayTags = FAuraGameplayTags::Get();
	
	FAuraAbilityInfo LastSlotInfo;
	LastSlotInfo.StatusTag =  GameplayTags.Abilities_Status_Unlocked;
	LastSlotInfo.InputTag =  PreviousSlotTag;
	LastSlotInfo.AbilityTag =  GameplayTags.Abilities_None;

	// broadcast empty info if Previous Slot is a valid slot. Only if equipping an already equipped spell
	AbilityInfoDelegate.Broadcast(LastSlotInfo);

	FAuraAbilityInfo Info = AbilityInfo->FindAbilityInfoForTag(AbilityTag);
	Info.StatusTag = Status;
	Info.InputTag = SlotTag;
	AbilityInfoDelegate.Broadcast(Info);

	StopWaitingForEquipDelegate.Broadcast(AbilityInfo->FindAbilityInfoForTag(AbilityTag).AbilityType);
	SpellGlobeReassignedDelegate.Broadcast(AbilityTag);
	GlobeDeselect();
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
