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

void USHealthComponent::TakeDamage(const float InDamage, const AActor* DamagedActor)
{
	if (!DamagedActor || bDead) return;


	UE_LOG(LogHealthComponent, Log, TEXT("Damage: %s"), *FString::SanitizeFloat(InDamage));

	CurrenHealth = FMath::Clamp(CurrenHealth - InDamage, 0.0f, MaxHealth);

	OnTakeDamage.Broadcast();
	OnHealthChanged.Broadcast(CurrenHealth);
	IsDead(CurrenHealth);
}

void USHealthComponent::GainHealth(const float InValue)
{
	CurrenHealth = FMath::Clamp(CurrenHealth + InValue, 0.0f, MaxHealth);
	OnHealthChanged.Broadcast(CurrenHealth);
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
