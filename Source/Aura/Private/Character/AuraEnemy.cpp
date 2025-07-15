// Aura by Vredinka


#include "Character/AuraEnemy.h"

#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "Aura/Aura.h"

AAuraEnemy::AAuraEnemy()
{
	GetMesh()->SetCollisionResponseToChannel(ECC_Visibility, ECR_Block);

	AbilitySystemComponent = CreateDefaultSubobject<UAuraAbilitySystemComponent>("AbilitySystemComponent");
	AbilitySystemComponent->SetIsReplicated(true);
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Minimal);

	AttributeSet = CreateDefaultSubobject<UAttributeSet>("AttributeSet");	
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
}


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

void AAuraEnemy::InitAbilityActorInfo()
{	
	AbilitySystemComponent->InitAbilityActorInfo(this, this);
	Cast<UAuraAbilitySystemComponent>(AbilitySystemComponent)->AbilityActorInfoSet();
}
