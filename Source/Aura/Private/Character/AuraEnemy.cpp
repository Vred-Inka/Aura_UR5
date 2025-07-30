// Aura by Vredinka


#include "Character/AuraEnemy.h"

#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "AbilitySystem/AuraAttributeSet.h"
#include "Player/AuraPlayerState.h"
#include "Aura/Aura.h"
#include "Components/WidgetComponent.h"
#include "UI/Widget/AuraUserWidget.h"

AAuraEnemy::AAuraEnemy()
{
	GetMesh()->SetCollisionResponseToChannel(ECC_Visibility, ECR_Block);

	AbilitySystemComponent = CreateDefaultSubobject<UAuraAbilitySystemComponent>("AbilitySystemComponent");
	AbilitySystemComponent->SetIsReplicated(true);
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Minimal);

	AttributeSet = CreateDefaultSubobject<UAuraAttributeSet>("AttributeSet");
	
	HealthBar =  CreateDefaultSubobject<UWidgetComponent>("HealthBar");
	HealthBar->SetupAttachment(GetRootComponent());
}

void AAuraEnemy::HighlightActor()
{
	GetMesh()->SetRenderCustomDepth(true);
	GetMesh()->SetCustomDepthStencilValue(CUSTOM_DEPTH_RED);
	Weapon->SetRenderCustomDepth(true);
	Weapon->SetCustomDepthStencilValue(CUSTOM_DEPTH_RED);
}

void AAuraEnemy::UnhighlightActor()
{
	GetMesh()->SetRenderCustomDepth(false);
	Weapon->SetRenderCustomDepth(false);
}

int32 AAuraEnemy::GetPlayerLevel()
{
	return Level;
}

void AAuraEnemy::BeginPlay()
{
	Super::BeginPlay();

	InitAbilityActorInfo();
	
	if (UAuraUserWidget* AuraUserWidget = Cast<UAuraUserWidget>(HealthBar->GetUserWidgetObject()))
	{
		AuraUserWidget->SetWidgetController(this);
	}
	
	if (const UAuraAttributeSet* AuraAS =  CastChecked<UAuraAttributeSet>(AttributeSet))
	{
		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AuraAS->GetHealthAttribute()).AddLambda(
			[this](const FOnAttributeChangeData& Data)
			{
				OnHealthChanged.Broadcast(Data.NewValue);
			}
		);

		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AuraAS->GetMaxHealthAttribute()).AddLambda(
			[this](const FOnAttributeChangeData& Data)
			{
				OnMaxHealthChanged.Broadcast(Data.NewValue);
			}
		);

		OnHealthChanged.Broadcast(AuraAS->GetHealth());
		OnMaxHealthChanged.Broadcast(AuraAS->GetMaxHealth());
	}

}

void AAuraEnemy::InitAbilityActorInfo()
{
	AbilitySystemComponent->InitAbilityActorInfo(this, this);
	Cast<UAuraAbilitySystemComponent>(AbilitySystemComponent)->AbilityActorInfoSet();

	if (HasAuthority())
	{
		InitializeDefaultAttributes();		
	}
}


/*
 *
* 
template<typename ReturnType, typename... Args>
struct FunctionAdapterType
{
	using Type = ReturnType(*)(const FActiveGameplayEffectHandle&, Args&&...);
};
class AnyFakeFunctionPointer
{
};

using FunctionPointer = void*;
AnyFakeFunctionPointer m_Callback;
FunctionPointer m_CallbackAdapter{ nullptr };

template<typename... Args>
void Foo(Args&&... args)
{
	std::vector<FActiveGameplayEffectHandle> handlers;
	for (const FActiveGameplayEffectHandle& handler : handlers)
	{
		using AdapterFunctionType = typename FunctionAdapterType<int, Args...>::Type;
		AdapterFunctionType callbackAsFuncType = reinterpret_cast<AdapterFunctionType>(m_CallbackAdapter);

		const int result = callbackAsFuncType(handler, std::forward<Args>(args)...);
	}
}


class Body
{
public:
	float GetFilterInfo(){ return filterInfo; }
	void SetFilterInfo(float info){ filterInfo  =  info;}
	
private:
	float filterInfo {2.0f};
};

void Foo1(Body& body)
{
	const auto filterInfo = body.GetFilterInfo();

	for (int i = 0; i < 10; ++i)
	{
		Body b;
		b.SetFilterInfo(filterInfo);
	}
	
}

 */