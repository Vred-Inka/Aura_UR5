// Aura by Vredinka


#include "AbilitySystem/Abilities/AuraFireBlast.h"

#include "AbilitySystem/AuraAbilitySystemLibrary.h"
#include "EntitySystem/MovieSceneEntitySystemRunner.h"

FString UAuraFireBlast::GetDescription(int32 Level)
{
	const int32 ScaledDamage = Damage.GetValueAtLevel(Level);
	const float ManaCost = FMath::Abs(GetManaCost(Level));
	const float Cooldown = GetCooldown(Level);

	return FString::Printf(TEXT(
		"<Title>FIRE BLAST</>\n"

		// Level
		"<Small>Level: </><Level>%d</>\n"
		// ManaCost
		"<Small>Mana Cost: </><ManaCost>%.1f</>\n"
		// Cooldown
		"<Small>Cooldown: </><Cooldown>%.1f</>\n\n"

		// Number of Fire Balls
		"<Default>Lunches %d </>"
		"<Default>fire balls in all direction, each coming back and </>"
		"<Default>exploding upon return, causing </>"
		
		// Damage
		"<Damage>%d</><Default> radial fire damage with a chance to burn </>"
		),			
		Level,
		ManaCost,
		Cooldown,
		NumFireBalls,
		ScaledDamage);	
}

FString UAuraFireBlast::GetNextLevelDescription(int32 Level)
{
	const int32 ScaledDamage = Damage.GetValueAtLevel(Level + 1);
	const float ManaCost = FMath::Abs(GetManaCost(Level + 1));
	const float Cooldown = GetCooldown(Level + 1);

	return FString::Printf(TEXT(
		"<Title>NEXT LEVEL</>\n"

		// Level
		"<Small>Level: </><Level>%d</>\n"
		// ManaCost
		"<Small>Mana Cost: </><ManaCost>%.1f</>\n"
		// Cooldown
		"<Small>Cooldown: </><Cooldown>%.1f</>\n\n"

		// Number of Fire Balls
		"<Default>Lunches %d </>"
		"<Default>fire balls in all direction, each coming back and </>"
		"<Default>exploding upon return, causing </>"
		
		// Damage
		"<Damage>%d</><Default> radial fire damage with a chance to burn </>"
		),			
		Level,
		ManaCost,
		Cooldown,
		NumFireBalls,
		ScaledDamage);	
}

TArray<AAuraFireBall*> UAuraFireBlast::SpawnFireBalls()
{
	TArray<AAuraFireBall*> FireBalls;
	const FVector Forward = GetAvatarActorFromActorInfo()->GetActorForwardVector();
	const FVector Location = GetAvatarActorFromActorInfo()->GetActorLocation();
	TArray<FRotator> Rotations = UAuraAbilitySystemLibrary::EvenlySpacedRotators(Forward,
		FVector::UpVector, 360.f, NumFireBalls);

	for (const FRotator& Rotator : Rotations)
	{
		FTransform SpawnTransform;
		SpawnTransform.SetLocation(Location);
		SpawnTransform.SetRotation(Rotator.Quaternion());
		AAuraFireBall* FireBall = GetWorld()->SpawnActorDeferred<AAuraFireBall>(
			FireBallClass,
			SpawnTransform,
			GetOwningActorFromActorInfo(),
			CurrentActorInfo->PlayerController->GetPawn(),
			ESpawnActorCollisionHandlingMethod::AlwaysSpawn);

		FireBall->DamageEffectParams = MakeDamageEffectParamsFromClassDefaults();
		
		FireBalls.Add(FireBall);

		FireBall->FinishSpawning(SpawnTransform);
	}

	
	return TArray<AAuraFireBall*>();;
}
