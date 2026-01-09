#pragma once

#include "GameplayEffectTypes.h"
#include "AuraAbilityTypes.generated.h"

class UGameplayEffect;

USTRUCT(BlueprintType)
struct FDamageEffectParams
{
	GENERATED_BODY()

	FDamageEffectParams(){};

	UPROPERTY(BlueprintReadWrite)
	TObjectPtr<UObject> WorldContextObject = nullptr;

	UPROPERTY(BlueprintReadWrite)
	TSubclassOf<UGameplayEffect> DamageGameplayEffectClass = nullptr;

	UPROPERTY(BlueprintReadWrite)
	TObjectPtr<UAbilitySystemComponent> SourceAbilitySystemComponent;
	
	UPROPERTY(BlueprintReadWrite)
	TObjectPtr<UAbilitySystemComponent> TargetAbilitySystemComponent;

	UPROPERTY(BlueprintReadWrite)
	float BaseDamage{0.0f};

	UPROPERTY(BlueprintReadWrite)
	float AbilityLevel{1.0f};

	UPROPERTY(BlueprintReadWrite)
	FGameplayTag DamageType = FGameplayTag();

	UPROPERTY(BlueprintReadWrite)
	float DebuffChance{0.0f};

	UPROPERTY(BlueprintReadWrite)
	float DebuffDamage{0.0f};

	UPROPERTY(BlueprintReadWrite)
	float DebuffDuration{0.0f};

	UPROPERTY(BlueprintReadWrite)
	float DebuffFrequency{0.0f};
	
	UPROPERTY(BlueprintReadWrite)
	float DeathImpulseMagnitude{0.0f};

	UPROPERTY(BlueprintReadWrite)
	FVector DeathImpulse{FVector::ZeroVector};

	UPROPERTY(BlueprintReadWrite)
	float KnockbackForceMagnitude{0.0f};

	UPROPERTY(BlueprintReadWrite)
	float KnockbackChance{0.0f};

	UPROPERTY(BlueprintReadWrite)
	FVector KnockbackForce{FVector::ZeroVector};

	UPROPERTY(BlueprintReadWrite)
	bool bIsRadialDamage{false};

	UPROPERTY(BlueprintReadWrite)
	float RadialDamageInnerRadius{0.0f};

	UPROPERTY(BlueprintReadWrite)
	float RadialDamageOuterRadius{0.0f};

	UPROPERTY(BlueprintReadWrite)
	FVector RadialDamageOrigin{FVector::ZeroVector};
};

USTRUCT(BlueprintType)
struct FAuraGameplayEffectContext :  public FGameplayEffectContext
{
	GENERATED_BODY()

public:
	bool IsCriticalHit() const { return bIsCriticalHit; }
	bool IsBlockedHit() const { return bIsBlockedHit; }
	bool IsSuccessfulDebuff() const { return bIsSuccessfulDebuff; }
	float GetDebuffDamage() const { return DebuffDamage; }
	float GetDebuffDuration() const { return DebuffDuration; }
	float GetDebuffFrequency() const { return DebuffFrequency; }
	TSharedPtr<FGameplayTag> GetDamageType() const { return DamageType; }
	FVector GetDeathImpulse() const { return DeathImpulse; }
	FVector GetKnockbackForce() const { return KnockbackForce; }
	bool IsRadialDamage() const { return bIsRadialDamage; }
	float GetRadialDamageInnerRadius() const { return RadialDamageInnerRadius; }
	float GetRadialDamageOuterRadius() const { return RadialDamageOuterRadius; }
	FVector GetRadialDamageOrigin() const { return RadialDamageOrigin; }

	void SetIsCriticalHit(bool bInIsCriticalHit) { this->bIsCriticalHit = bInIsCriticalHit; }
	void SetIsBlockedHit(bool bInIsBlockingHit) { this->bIsBlockedHit = bInIsBlockingHit; }
	void SetIsSuccessfulDebuff(bool bInIsSuccessfulDebuff) { this->bIsSuccessfulDebuff = bInIsSuccessfulDebuff; }
	void SetDebuffDamage(float InDamage) { this->DebuffDamage = InDamage; }
	void SetDebuffDuration(float InDuration) { this->DebuffDuration = InDuration; }
	void SetDebuffFrequency(float InFrequency) { this->DebuffFrequency = InFrequency; }
	void SetDamageType(TSharedPtr<FGameplayTag> InDamageType) { this->DamageType = InDamageType; }
	void SetDeathImpulse(const FVector& InDeathImpulse) { this->DeathImpulse = InDeathImpulse; }
	void SetKnockbackForce(const FVector& InForce) { this->KnockbackForce = InForce; }
	void SetIsRadialDamage(bool bInIsRadialDamage) { this->bIsRadialDamage = bInIsRadialDamage; }
	void SetRadialDamageInnerRadius(float InInnerRadius) { this->RadialDamageInnerRadius = InInnerRadius; }
	void SetRadialDamageOuterRadius(float InOuterRadius) { this->RadialDamageOuterRadius = InOuterRadius; }
	void SetRadialDamageOrigin(const FVector& InOrigin) { this->RadialDamageOrigin = InOrigin; }
	
	virtual UScriptStruct* GetScriptStruct() const override
	{
		return StaticStruct();
	}

	virtual FAuraGameplayEffectContext* Duplicate() const
	{
		FAuraGameplayEffectContext* NewContext = new FAuraGameplayEffectContext();
		*NewContext = *this;
		if (GetHitResult())
		{
			//Does a deep copy of the hit result
			NewContext->AddHitResult(*GetHitResult(), true);
		}
		return NewContext;
	}

	virtual bool NetSerialize(FArchive& Ar, class UPackageMap* Map, bool& bOutSuccess) override;

protected:

	UPROPERTY()
	bool bIsBlockedHit = false;

	UPROPERTY()
	bool bIsCriticalHit = false;

	UPROPERTY()
	bool bIsSuccessfulDebuff = false;

	UPROPERTY()
	float DebuffDamage = 0.0f;
	
	UPROPERTY()
	float DebuffDuration = 0.0f;

	UPROPERTY()
	float DebuffFrequency = 0.0f;

	TSharedPtr<FGameplayTag> DamageType;

	UPROPERTY()
	FVector DeathImpulse =  FVector::ZeroVector;

	UPROPERTY()
	FVector KnockbackForce = FVector::ZeroVector;
	
	UPROPERTY()
	bool bIsRadialDamage{false};

	UPROPERTY()
	float RadialDamageInnerRadius{0.0f};

	UPROPERTY()
	float RadialDamageOuterRadius{0.0f};

	UPROPERTY()
	FVector RadialDamageOrigin{FVector::ZeroVector};	
};

template<>
struct TStructOpsTypeTraits<FAuraGameplayEffectContext> : public TStructOpsTypeTraitsBase2<FAuraGameplayEffectContext>
{
	enum
	{
		WithNetSerializer = true,
		WithCopy = true
	};
};