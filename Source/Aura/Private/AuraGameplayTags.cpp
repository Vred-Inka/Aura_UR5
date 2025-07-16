// Aura by Vredinka


#include "AuraGameplayTags.h"
#include "GameplayTagsManager.h"

FAuraGameplayTags FAuraGameplayTags::GameplayTags;

void FAuraGameplayTags::InitializeNativeGameplayTags()
{
	 UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attribute.Secondary.Armor")
	 	, FString("Reduces damage taken, improves Block Chance."));
}
