// Aura by Vredinka

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "NiagaraComponent.h"
#include "DebuffNiagaraComponent.generated.h"

/**
 * 
 */
UCLASS()
class AURA_API UDebuffNiagaraComponent : public UNiagaraComponent
{
	GENERATED_BODY()

public:
	UDebuffNiagaraComponent();
	
	UPROPERTY(EditAnywhere)
	FGameplayTag DebuffTag;

protected:
	virtual void BeginPlay() override;
	void DebuffTagChanged(const FGameplayTag InDebuffTag, int32 NewCount);

	UFUNCTION()
	void OnOwnerDeath(AActor* DeadActor);
	
};
