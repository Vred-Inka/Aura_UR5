// Aura by Vredinka


#include "Actor/AuraFireBall.h"

void AAuraFireBall::OnSphereOverlap(UPrimitiveComponent* OverlapedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	
}

void AAuraFireBall::BeginPlay()
{
	Super::BeginPlay();
	StartOutgoingTimeline();
}
