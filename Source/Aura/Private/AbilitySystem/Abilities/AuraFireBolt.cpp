// Aura by Vredinka


#include "AbilitySystem/Abilities/AuraFireBolt.h"

#include "Interaction/CombatInterface.h"
#include "Kismet/KismetSystemLibrary.h"
#include "WorldPartition/Cook/WorldPartitionCookPackage.h"

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

void UAuraFireBolt::SpawnProjectiles(const FVector& TargetLocation, const FGameplayTag& SocketTag, bool bOverridePitch,
	float PitchOverride, AActor* HomingTarget)
{
	const bool bIsServer = GetAvatarActorFromActorInfo()->HasAuthority();

	if (!bIsServer)
		return;

	const FVector SocketLocation =  ICombatInterface::Execute_GetCombatSocketLocation(
		GetAvatarActorFromActorInfo(),
		SocketTag);
	FRotator Rotation = (TargetLocation - SocketLocation).Rotation();

	if (bOverridePitch)
	{
		Rotation.Pitch = PitchOverride;
	}
	
	const FVector Forward = Rotation.Vector();
	const FVector LeftOfSpread = Forward.RotateAngleAxis(-ProjectileSpread/2.0f, FVector::UpVector);
	const FVector RightOfSpread = Forward.RotateAngleAxis(ProjectileSpread/2.0f, FVector::UpVector);

	//NumProjectiles = FMath::Min(this->MaxProjectilesNum, GetAbilityLevel());
	if (NumProjectiles > 1)
	{
		const float DeltaSpread = ProjectileSpread / (NumProjectiles-1);
		for (int32 i = 0;  i < NumProjectiles; i++)
		{
			const FVector Direction = LeftOfSpread.RotateAngleAxis(i * DeltaSpread, FVector::UpVector);

			const FVector Start =  SocketLocation + FVector(0.0f, 0.0f, 5.f);
			UKismetSystemLibrary::DrawDebugArrow(GetAvatarActorFromActorInfo(),
				Start,
				Start +  Direction * 75.f,
				5, FLinearColor::Red, 120, 2 );
		}
	}
	else
	{
		const FVector Start =  SocketLocation + FVector(0.0f, 0.0f, 5.f);
		UKismetSystemLibrary::DrawDebugArrow(GetAvatarActorFromActorInfo(),
			Start,
			Start +  Forward * 75.f,
			5, FLinearColor::Red, 120, 2 );
		
	}

	UKismetSystemLibrary::DrawDebugArrow(GetAvatarActorFromActorInfo(), SocketLocation,
		SocketLocation +  Rotation.Vector() *100.f, 5, FLinearColor::White, 120, 2 );

	UKismetSystemLibrary::DrawDebugArrow(GetAvatarActorFromActorInfo(), SocketLocation,
	SocketLocation +  LeftOfSpread *100.f, 5, FLinearColor::Gray, 120, 2 );

	
	UKismetSystemLibrary::DrawDebugArrow(GetAvatarActorFromActorInfo(), SocketLocation,
	SocketLocation +  RightOfSpread *100.f, 5, FLinearColor::Gray, 120, 2 );
}
