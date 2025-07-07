#include "Shooter/Public/Components/SHealthComponent.h"


// Log category for HealthComponent
DEFINE_LOG_CATEGORY(LogHealthComponent);

USHealthComponent::USHealthComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}


void USHealthComponent::BeginPlay()
{
	Super::BeginPlay();

	Initialize();
}


void USHealthComponent::Initialize()
{
	CurrenHealth = MaxHealth;
}

void USHealthComponent::TakeDamage(const float InDamage, AActor* DamagedActor)
{
	if (!DamagedActor) return;

	if (!bDead)
	{
		UE_LOG(LogHealthComponent, Log, TEXT("Damage: %s"), *FString::SanitizeFloat(InDamage));

		CurrenHealth = FMath::Clamp(CurrenHealth - InDamage, 0.0f, MaxHealth);
		OnHealthChanged.Broadcast(CurrenHealth);
		IsDead(CurrenHealth);
	}
}


bool USHealthComponent::IsDead(const float InHealth)
{
	if (InHealth <= 0.0f)
	{
		bDead = true;
		OnDead.Broadcast();
	}

	return bDead;
}
