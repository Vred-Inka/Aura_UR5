// Aura by Vredinka


#include "AbilitySystem/Abilities/AuraSummonAbility.h"

#include "NiagaraFunctionLibrary.h"
#include "Kismet/KismetSystemLibrary.h"

TArray<FVector> UAuraSummonAbility::GetSpawnLocations()
{
	const FVector Forward = GetAvatarActorFromActorInfo()->GetActorForwardVector();
	const FVector Location = GetAvatarActorFromActorInfo()->GetActorLocation();
	const float DeltaSpread = SpawnSpread/ NumMinions;

	const FVector LeftOfSpread = Forward.RotateAngleAxis(-SpawnSpread/2.0f, FVector::UpVector);
	TArray<FVector> SpawnLocations;
	for (int32 i = 0;  i < NumMinions; i++)
	{
		const FVector Direction = LeftOfSpread.RotateAngleAxis(i * DeltaSpread, FVector::UpVector);
		FVector ChosenSpawnLocation =  Location + Direction * FMath::RandRange(MinSpawnDistance, MaxSpawnDistance);

		FHitResult Hit;
		GetWorld()->LineTraceSingleByChannel(Hit,  ChosenSpawnLocation + FVector(0.0f, 0.0f, 400.0f),ChosenSpawnLocation - FVector(0.0f, 0.0f, 400.0f), ECC_Visibility);

		if (Hit.bBlockingHit)
		{
			ChosenSpawnLocation =  Hit.ImpactPoint;
		}
		
		SpawnLocations.Add(ChosenSpawnLocation);
				
		UKismetSystemLibrary::DrawDebugArrow(
			GetAvatarActorFromActorInfo(),
			Location,
			Location + Direction * MaxSpawnDistance,
			4.0,
			FLinearColor::Green,
			3.0f
		);

		//USceneComponent* NiagaraComponent ;
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), this, NAME_None,
			ChosenSpawnLocation, FRotator(0.f), EAttachLocation::Type::KeepRelativeOffset, true);
		
	}

	return SpawnLocations;
}

TSubclassOf<APawn> UAuraSummonAbility::GetRandomMinionClass()
{
	const int32 Selection =  FMath::RandRange(0, MinionClasses.Num()-1);
	return MinionClasses[Selection];
}
