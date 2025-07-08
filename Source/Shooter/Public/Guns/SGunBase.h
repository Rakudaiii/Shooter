#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "SGunBase.generated.h"


// Log category for Gun
DECLARE_LOG_CATEGORY_EXTERN(LogGun, Log, All);

UCLASS(Blueprintable)
class SHOOTER_API USGunBase : public UObject
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	virtual void Init(ACharacter* InCharacter);

	UFUNCTION(BlueprintCallable)
	void StartFiring();

	UFUNCTION(BlueprintCallable)
	void StopFiring();

	UFUNCTION(BlueprintCallable)
	void ReloadAmmo();

protected:
	// === Owner ===
	UPROPERTY()
	ACharacter* OwnerCharacterGun;

	// === Firing ===
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Gun|Fire")
	bool bIsAuto = true;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Gun|Fire")
	float FireRate = 0.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Gun|Fire")
	float GunDamage = 0.0f;

	UPROPERTY(BlueprintReadOnly, Category="Gun|Fire")
	bool bIsFiring = false;


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

	UPROPERTY()
	FTimerHandle ReloadHandle;

	UFUNCTION()
	void CompleteReload();

public:
	// === Getters ===
	UFUNCTION(BlueprintPure)
	FORCEINLINE int32 GetCurrentAmmo() const { return CurrentAmmo; }

	UFUNCTION(BlueprintPure)
	FORCEINLINE int32 GetMaxAmmo() const { return MaxAmmo; }

	UFUNCTION(BlueprintPure)
	FORCEINLINE bool IsReloading() const { return bIsReloading; }

	UFUNCTION(BlueprintPure)
	FORCEINLINE float GetGunDamage() const { return GunDamage; }

	UFUNCTION(BlueprintPure)
	FORCEINLINE ACharacter* GetOwnerCharacter() const { return OwnerCharacterGun; }

	UFUNCTION(BlueprintPure)
	FORCEINLINE float GetFireRate() const { return FireRate; }

	UFUNCTION(BlueprintPure)
	FORCEINLINE bool IsFiring() const { return bIsFiring; }
};
