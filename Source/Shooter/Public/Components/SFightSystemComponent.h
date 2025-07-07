#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "SFightSystemComponent.generated.h"

class USGunBase;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class SHOOTER_API USFightSystemComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	USFightSystemComponent();

	UFUNCTION(BlueprintCallable, Category = "Shoot")
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
	ACharacter* OwnerCharacter;

	UFUNCTION(BlueprintCallable, Category = "Component")
	void Initialize();


	// ------------ Shoot ------------
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Abilities")
	bool bCanShoot = true;

	UPROPERTY()
	USGunBase* CurrentGun;

public:
	UFUNCTION(BlueprintPure) FORCEINLINE USGunBase* GetCurrentGun() const { return CurrentGun; };
	UFUNCTION(BlueprintPure) FORCEINLINE bool GetbCanShoot() const { return bCanShoot; };
};
