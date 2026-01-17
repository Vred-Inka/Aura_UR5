// Aura by Vredinka


#include "Game/AuraGameModeBase.h"

#include "Game/LoadScreenSaveGame.h"
#include "Kismet/GameplayStatics.h"
#include "UI/ViewModels/MVVM_LoadSlot.h"

void AAuraGameModeBase::SaveSlotData(UMVVM_LoadSlot& LoadSlot, int32 SlotIndex)
{
	if (UGameplayStatics::DoesSaveGameExist(LoadSlot.GetLoadSlotName(), SlotIndex))
	{
		UGameplayStatics::DeleteGameInSlot(LoadSlot.GetLoadSlotName(), SlotIndex);
	}
	
	USaveGame* SaveGameobject = UGameplayStatics::CreateSaveGameObject(LoadScreenSaveGameClass);
	ULoadScreenSaveGame* LoadScreenSaveGame = Cast<ULoadScreenSaveGame>(SaveGameobject);
	LoadScreenSaveGame->PlayerName = LoadSlot.GetPlayerName();

	UGameplayStatics::SaveGameToSlot(LoadScreenSaveGame, LoadSlot.GetLoadSlotName(), SlotIndex);
}
