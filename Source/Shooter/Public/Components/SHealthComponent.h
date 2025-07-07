#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "SHealthComponent.generated.h"

//Delegates
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnDeadDelegate);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnHealthChangedDelegate, const float, CurrentHealth);

// Log category for HealthComponent
DECLARE_LOG_CATEGORY_EXTERN(LogHealthComponent, Log, All);

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class SHOOTER_API USHealthComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	USHealthComponent();

	UFUNCTION(BlueprintCallable)
	void Initialize();

	UFUNCTION(BlueprintCallable)
	void TakeDamage(const float InDamage, AActor* DamagedActor);
	
	UPROPERTY(BlueprintAssignable, Category = "Events")
	FOnDeadDelegate OnDead;

	UPROPERTY(BlueprintAssignable, Category = "Events")
	FOnHealthChangedDelegate OnHealthChanged;

	UFUNCTION(BlueprintCallable)
	bool IsDead(const float InHealth);

protected:
	virtual void BeginPlay() override;

	UPROPERTY()
	float CurrenHealth;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	float MaxHealth;

	UPROPERTY()
	bool bDead = false;

public:
	UFUNCTION(BlueprintPure) FORCEINLINE float GetCurrentHealth() const { return CurrenHealth; }
	UFUNCTION(BlueprintPure) FORCEINLINE float GetMaxHealth() const { return MaxHealth; }
	UFUNCTION(BlueprintPure) FORCEINLINE bool GetIsDead() const { return bDead; }
	
};
