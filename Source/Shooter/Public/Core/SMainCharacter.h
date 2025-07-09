#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Logging/LogMacros.h"
#include "SMainCharacter.generated.h"


//Input
class UInputMappingContext;
class UInputAction;
struct FInputActionValue;


//Components
class UCameraComponent;

class USHealthComponent;
class USParkourMovementComponent;
class USBuffManager;
class USWeaponManager;

// Log category for Main Character
DECLARE_LOG_CATEGORY_EXTERN(LogMainCharacter, Log, All);

UCLASS()
class SHOOTER_API ASMainCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	ASMainCharacter();

protected:
	virtual void BeginPlay() override;

	void Move(const FInputActionValue& Value);

	void Look(const FInputActionValue& Value);

	/// --------- Input ---------
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	UInputMappingContext* DefaultMappingContext;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	UInputAction* JumpAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	UInputAction* MoveAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	UInputAction* LookAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	UInputAction* SprintAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	UInputAction* SlideAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	UInputAction* ShootAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	UInputAction* ReloadAction;

	virtual void NotifyControllerChanged() override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

protected:
	// --------- Components ---------
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Component")
	UCameraComponent* Camera;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Component")
	USHealthComponent* HealthComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Component")
	USParkourMovementComponent* ParkourMovementComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Component")
	USWeaponManager* WeaponManager;

public:
	// --------- Getters ---------
	UFUNCTION(BlueprintPure)FORCEINLINE UCameraComponent* GetCamera() const { return Camera; }
	UFUNCTION(BlueprintPure)FORCEINLINE USHealthComponent* GetHealthComponent() const { return HealthComponent; }
	UFUNCTION(BlueprintPure)FORCEINLINE USParkourMovementComponent* GetParkourMovementComponent() const { return ParkourMovementComponent; }
	UFUNCTION(BlueprintPure)FORCEINLINE USWeaponManager* GetWeaponManager() const { return WeaponManager; }
};
