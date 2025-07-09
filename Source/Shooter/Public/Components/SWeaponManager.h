#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "SWeaponManager.generated.h"

class USGunBase;

// Log category for FightSystemComponent
DECLARE_LOG_CATEGORY_EXTERN(LogWeaponManager, Log, All);

UCLASS(Blueprintable, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class SHOOTER_API USWeaponManager : public UActorComponent
{
	GENERATED_BODY()

public:
	USWeaponManager();

	UFUNCTION(BlueprintCallable)
	void EquipGun(TSubclassOf<USGunBase> GunClass);

	UFUNCTION(BlueprintCallable)
	void StartFiring();

	UFUNCTION(BlueprintCallable)
	void StopFiring();

	UFUNCTION(BlueprintCallable)
	void ReloadGun();

protected:
	virtual void BeginPlay() override;

	// ------------ Component ------------
	UPROPERTY(BlueprintReadOnly)
	AActor* OwnerActor;

	UFUNCTION(BlueprintCallable, Category = "Component")
	void Initialize();

	// ------------ Shoot ------------
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Abilities")
	bool bCanShoot = true;

	UPROPERTY()
	USGunBase* CurrentGun;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<USGunBase> SelectedGun;

public:
	UFUNCTION(BlueprintPure) FORCEINLINE USGunBase* GetCurrentGun() const { return CurrentGun; };
	UFUNCTION(BlueprintPure) FORCEINLINE bool GetbCanShoot() const { return bCanShoot; };
};
