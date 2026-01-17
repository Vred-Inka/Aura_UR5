// Aura by Vredinka


#include "Game/AuraGameModeBase.h"

#include "Game/LoadScreenSaveGame.h"
#include "Kismet/GameplayStatics.h"
#include "UI/ViewModels/MVVM_LoadSlot.h"

void AAuraGameModeBase::SaveSlotData(UMVVM_LoadSlot& LoadSlot, int32 SlotIndex)
{
	DeleteSlot(LoadSlot.GetLoadSlotName(), SlotIndex);
	
	USaveGame* SaveGameobject = UGameplayStatics::CreateSaveGameObject(LoadScreenSaveGameClass);
	ULoadScreenSaveGame* LoadScreenSaveGame = Cast<ULoadScreenSaveGame>(SaveGameobject);
	LoadScreenSaveGame->PlayerName = LoadSlot.GetPlayerName();
	LoadScreenSaveGame->SaveSlotStatus = Taken;
	LoadScreenSaveGame->MapName = LoadSlot.GetMapName();
	LoadScreenSaveGame->PlayerLevel = LoadSlot.GetPlayerLevel();

	UGameplayStatics::SaveGameToSlot(LoadScreenSaveGame, LoadSlot.GetLoadSlotName(), SlotIndex);
}

ULoadScreenSaveGame* AAuraGameModeBase::GetSaveSlotData(const FString& SlotName, int32 SlotIndex) const
{
	USaveGame* SaveGameobject = nullptr;
	if (UGameplayStatics::DoesSaveGameExist(SlotName, SlotIndex))
	{
		SaveGameobject = UGameplayStatics::LoadGameFromSlot(SlotName, SlotIndex);
	}
	else
	{
		SaveGameobject = UGameplayStatics::CreateSaveGameObject(LoadScreenSaveGameClass);
	}

	ULoadScreenSaveGame* LoadScreenSaveGame = Cast<ULoadScreenSaveGame>(SaveGameobject);
	return LoadScreenSaveGame;
}

void AAuraGameModeBase::DeleteSlot(const FString& SlotName, int32 SlotIndex)
{
	if (UGameplayStatics::DoesSaveGameExist(SlotName, SlotIndex))
	{
		UGameplayStatics::DeleteGameInSlot(SlotName, SlotIndex);
	}
}

void AAuraGameModeBase::TravelToMap(UMVVM_LoadSlot* Slot)
{
	const FString MapName = Slot->GetMapName();
	const int32 SlotIndex = Slot->SlotIndex;
	UGameplayStatics::OpenLevelBySoftObjectPtr(Slot,Worlds.FindChecked(MapName) );
}

void AAuraGameModeBase::BeginPlay()
{
	Super::BeginPlay();
	Worlds.Add(DefaultWorldName, DefaultWorld);
}
