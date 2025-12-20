// Aura by Vredinka


#include "AbilitySystem/Abilities/AuraFireBolt.h"

#include "AbilitySystem/AuraAbilitySystemLibrary.h"
#include "Actor/AuraProjectile.h"
#include "Interaction/CombatInterface.h"

FString UAuraFireBolt::GetDescription(int32 Level)
{
	const int32 ScaledDamage = Damage.GetValueAtLevel(Level);
	const float ManaCost = FMath::Abs(GetManaCost(Level));
	const float Cooldown = GetCooldown(Level);
	if (Level == 1)
	{
		return FString::Printf(TEXT(
			"<Title>FIRE BOLT</>\n"

			// Level
			"<Small>Level: </><Level>%d</>\n"
			// ManaCost
			"<Small>Mana Cost: </><ManaCost>%.1f</>\n"
			// Cooldown
			"<Small>Cooldown: </><Cooldown>%.1f</>\n\n"

			//	Number of Fire Bolts
   			"<Default>Lunches a bolt of fire, exploding on impact and dealing:</>"
   			// Damage
			"<Damage>%d</><Default> fire damage with a chance to burn </>"
			),			
			Level,
			ManaCost,
			Cooldown,
			ScaledDamage);
	}
	else
	{
		return FString::Printf(TEXT(
			"<Title>FIRE BOLT</>\n"

			// Level
			"<Small>Level: </><Level>%d</>\n"
			// ManaCost
			"<Small>Mana Cost: </><ManaCost>%.1f</>\n"
			// Cooldown
			"<Small>Cooldown: </><Cooldown>%.1f</>\n\n"

			// Number of Fire Bolts
			"<Default>Lunches %d bolts of fire, exploding on impact and dealing:</>"
			// Damage
			"<Damage>%d</><Default> fire damage with a chance to burn </>"		
			),
			Level,
			ManaCost,
			Cooldown,
			FMath::Min(Level, NumProjectiles),
			ScaledDamage);
	}
}

FString UAuraFireBolt::GetNextLevelDescription(int32 Level)
{
	const int32 ScaledDamage = Damage.GetValueAtLevel(Level);
	const float ManaCost = FMath::Abs(GetManaCost(Level + 1));
	const float Cooldown = GetCooldown(Level + 1);
	return  FString::Printf(TEXT(
		"<Title>NEXT LEVEL: </>\n"

		// Level
		"<Small>Level: </><Level>%d</>\n"
		// Mana Cost
		"<Small>Mana Cost: </><ManaCost>%.1f</>\n"
		// Cooldown
		"<Small>Cooldown: </><Cooldown>%.1f</>\n\n"

		// Number of Fire Bolts
		"<Default>Lunches %d bolts of fire, exploding on impact and dealing:</>"
		// Damage
		"<Damage>%d</><Default> fire damage with a chance to burn </>"		
		),
		Level,
		ManaCost,
		Cooldown,
		FMath::Min(Level, NumProjectiles),
		ScaledDamage);
}

void UAuraFireBolt::SpawnProjectiles(const FVector& ProjectileTargetLocation, const FGameplayTag& SocketTag,
	bool bOverridePitch, float PitchOverride, AActor* HomingTarget)
{
	const bool bIsServer = GetAvatarActorFromActorInfo()->HasAuthority();

	if (!bIsServer)
		return;

	const FVector SocketLocation =  ICombatInterface::Execute_GetCombatSocketLocation(
		GetAvatarActorFromActorInfo(),
		SocketTag);
	FRotator Rotation = (ProjectileTargetLocation - SocketLocation).Rotation();

	if (bOverridePitch)
	{
		Rotation.Pitch = PitchOverride;
	}

	const FVector Forward =  Rotation.Vector();

	TArray<FRotator> Rotations =  UAuraAbilitySystemLibrary::EvenlySpacedRotators(Forward, FVector::UpVector, ProjectileSpread, NumProjectiles);

	for (FRotator& Rot : Rotations)
	{
		FTransform SpawnTransform;
		SpawnTransform.SetLocation(SocketLocation);
		SpawnTransform.SetRotation(Rot.Quaternion());
		AAuraProjectile* Projectile = GetWorld()->SpawnActorDeferred<AAuraProjectile>(
			ProjectileClass,
			SpawnTransform,
			GetOwningActorFromActorInfo(),
			Cast<APawn>(GetOwningActorFromActorInfo()),
			ESpawnActorCollisionHandlingMethod::AlwaysSpawn);

		Projectile->DamageEffectParams = MakeDamageEffectParamsFromClassDefaults();
	
		Projectile->FinishSpawning(SpawnTransform);
	}
/*
	const FVector LeftOfSpread = Forward.RotateAngleAxis(-ProjectileSpread/2.0f, FVector::UpVector);
	const FVector RightOfSpread = Forward.RotateAngleAxis(ProjectileSpread/2.0f, FVector::UpVector);

	if (NumProjectiles > 1)
	{
		const float DeltaSpread =  ProjectileSpread / (NumProjectiles - 1);
		for (int32 i = 0; i < NumProjectiles; ++i)
		{
			const FVector Direction = LeftOfSpread.RotateAngleAxis(DeltaSpread * i, FVector::UpVector);
			const FVector Start =  SocketLocation + FVector(0.0f, 0.0f, 5.0f);
			UKismetSystemLibrary::DrawDebugArrow(
				GetAvatarActorFromActorInfo(),
				Start,
				Start + Direction * 75.0f,
				1,
				FLinearColor::Red,
				120,
				1
			);
		}
	}
	else
	{
		const FVector Start =  SocketLocation + FVector(0.0f, 0.0f, 5.0f);
		UKismetSystemLibrary::DrawDebugArrow(
			GetAvatarActorFromActorInfo(),
			Start,
			Start + Forward * 75.0f,
			1,
			FLinearColor::Red,
			120,
			1
		);
	}
	
	UKismetSystemLibrary::DrawDebugArrow(GetAvatarActorFromActorInfo(),
		SocketLocation, SocketLocation + Forward * 100.0f, 1, FLinearColor::White, 120, 1);
	UKismetSystemLibrary::DrawDebugArrow(GetAvatarActorFromActorInfo(),
		SocketLocation, SocketLocation + LeftOfSpread * 100.0f, 1, FLinearColor::Gray, 120, 1);
	UKismetSystemLibrary::DrawDebugArrow(GetAvatarActorFromActorInfo(),
		SocketLocation, SocketLocation + RightOfSpread * 100.0f, 1, FLinearColor::Gray, 120, 1);
*/

}
