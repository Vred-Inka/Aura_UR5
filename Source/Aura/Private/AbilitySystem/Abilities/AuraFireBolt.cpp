// Aura by Vredinka


#include "AbilitySystem/Abilities/AuraFireBolt.h"

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