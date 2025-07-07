#pragma once

#include "CoreMinimal.h"
#include "Shooter/Public/Effects/SEffectBase.h"
#include "SGunBase.generated.h"


// Log category for Gun
DECLARE_LOG_CATEGORY_EXTERN(LogGun, Log, All);

UCLASS(Blueprintable)
class SHOOTER_API USGunBase : public USEffectBase
{
	GENERATED_BODY()

public:
	virtual void ApplyEffect() override;

	UFUNCTION(BlueprintCallable)
	void Init(ACharacter* InCharacter);

	UFUNCTION(BlueprintCallable)
	void StartFiring();

	UFUNCTION(BlueprintCallable)
	void StopFiring();

	UFUNCTION(BlueprintCallable)
	void ReloadAmmo();

	

protected:
	// === Owner ===
	UPROPERTY() ACharacter* OwnerCharacterGun;

	// === Firing ===
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Gun|Fire")
	bool bIsAuto = false;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Gun|Fire")
	float FireRate = 0.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Gun|Fire")
	float GunDamage = 0.0f;

	
	

	UPROPERTY()
	FTimerHandle AutoFireHandle;

	UFUNCTION()
	void Fire();

	// === Reloading ===
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Gun|Reload")
	float ReloadTime = 1.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Gun|Reload")
	int32 MaxAmmo = 6;

	UPROPERTY(BlueprintReadOnly, Category="Gun|Reload")
	int32 CurrentAmmo = 0;

	UPROPERTY(BlueprintReadOnly, Category="Gun|Reload")
	bool bIsReloading = false;

	UPROPERTY(BlueprintReadOnly, Category="Gun|Reload")
	bool bIsManualReloading = false;

	UPROPERTY()
	FTimerHandle ReloadHandle;

	UFUNCTION()
	void CompleteReload();

	// === Gun Data ===
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Gun")
	TSubclassOf<USGunBase> GunClass;



public:
	// === Getters ===
	UFUNCTION(BlueprintPure) FORCEINLINE int32 GetCurrentAmmo() const { return CurrentAmmo; }
	UFUNCTION(BlueprintPure) FORCEINLINE int32 GetMaxAmmo() const { return MaxAmmo; }
	UFUNCTION(BlueprintPure) FORCEINLINE bool IsReloading() const { return bIsReloading; }
	UFUNCTION(BlueprintPure) FORCEINLINE float GetGunDamage() const { return GunDamage; }
	UFUNCTION(BlueprintPure) FORCEINLINE ACharacter* GetOwnerCharacter() const { return OwnerCharacterGun; }
	UFUNCTION(BlueprintPure) FORCEINLINE float GetFireRate() const { return FireRate; }
	UFUNCTION(BlueprintPure) FORCEINLINE float GetTimeRemaining() const
	{
		return GetWorld()->GetTimerManager().IsTimerActive(ReloadHandle)
			? FMath::RoundToFloat(GetWorld()->GetTimerManager().GetTimerRemaining(ReloadHandle) * 10.f) / 10.f
			: 0.f;
	}
};
