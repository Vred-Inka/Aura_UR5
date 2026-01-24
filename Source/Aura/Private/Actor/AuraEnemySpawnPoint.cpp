// Aura by Vredinka


#include "Actor/AuraEnemySpawnPoint.h"

#include "Character/AuraEnemy.h"

void AAuraEnemySpawnPoint::SpawnEnemy()
{
	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

	AAuraEnemy* Enemy =  GetWorld()->SpawnActorDeferred<AAuraEnemy>(EnemyClass, GetActorTransform());
	Enemy->SetCharacterClass(EnemyCharacterClass);
	Enemy->SetLevel(EnemyLevel);
	Enemy->FinishSpawning(GetActorTransform());
	Enemy->SpawnDefaultController();
	
}
