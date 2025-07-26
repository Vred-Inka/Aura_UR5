// Aura by Vredinka


#include "AbilitySystem/Abilities/AuraProjectileSpell.h"

#include "Actor/AuraProjectile.h"
#include "Interaction/CombatInterface.h"

void UAuraProjectileSpell::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                           const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
                                           const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
}

void UAuraProjectileSpell::SpawnProjectile()
{
	const bool bIsServer = GetAvatarActorFromActorInfo()->HasAuthority();

	if (!bIsServer)
		return;

	ICombatInterface* CombatInterface =  Cast<ICombatInterface>(GetAvatarActorFromActorInfo());

	if (CombatInterface != nullptr)
	{
		const FVector SocketLocation =  CombatInterface->GetCombatSocketLocation();
		FTransform SpawnTransform;
		SpawnTransform.SetLocation(SocketLocation);

		//TODO : Set the projectile direction
		
		AAuraProjectile* Projectile = GetWorld()->SpawnActorDeferred<AAuraProjectile>(
			ProjectileClass,
			SpawnTransform,
			GetOwningActorFromActorInfo(),
			Cast<APawn>(GetOwningActorFromActorInfo()),
			ESpawnActorCollisionHandlingMethod::AlwaysSpawn);

		//Give projectile a gameplay Effect Spec for causing Damage
		Projectile->FinishSpawning(SpawnTransform);
	}
}
