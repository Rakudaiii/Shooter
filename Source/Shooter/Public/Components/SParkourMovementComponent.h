#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "SParkourMovementComponent.generated.h"

class UCharacterMovementComponent;
class ACharacter;

// Log category for parkour movement
DECLARE_LOG_CATEGORY_EXTERN(LogParkourMovementComponent, Log, All);

UCLASS(Blueprintable, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class SHOOTER_API USParkourMovementComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	USParkourMovementComponent();

	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;

protected:
	virtual void BeginPlay() override;
	
	UFUNCTION()
	void Initialize();
	
	// References 
	UPROPERTY(BlueprintReadOnly, Category = "Component")
	ACharacter* OwnerCharacter;

	UPROPERTY(BlueprintReadOnly, Category = "Component")
	UCharacterMovementComponent* CharacterMovementComponent;
	
	UFUNCTION(BlueprintCallable, Category = "Component")
	ACharacter* GetOwnerCharacter() const { return OwnerCharacter; }

	UFUNCTION(BlueprintCallable, Category = "Component")
	UCharacterMovementComponent* GetCharacterMovementComponent() const { return CharacterMovementComponent; }

	// --------- Slide ---------
public:
	UFUNCTION(BlueprintCallable, Category = "Slide")
	void Slide();

protected:
	UFUNCTION(BlueprintCallable, Category = "Slide")
	void InitializeSlide();

	UFUNCTION(BlueprintCallable, Category = "Slide")
	void StartSlide();

	UFUNCTION(BlueprintCallable, Category = "Slide")
	void StopSlide();

	UFUNCTION(BlueprintCallable, Category = "Slide")
	void StandUpAnimation(float DeltaTime);

	// Slide properties
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Abilities")
	bool bCanSlide = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Slide")
	float SlideDuration = 0.8f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Slide")
	float SlideSpeed = 800.0f;

	UPROPERTY()
	float SlideFriction = 0.1f;

	UPROPERTY()
	float SlideDeceleration = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Slide")
	float StandUpSpeed = 10.0f;

	// Getters for slide properties
	UFUNCTION(BlueprintCallable, Category = "Slide")
	bool CanSlide() const { return bCanSlide; }

	UFUNCTION(BlueprintCallable, Category = "Slide")
	float GetSlideDuration() const { return SlideDuration; }

	UFUNCTION(BlueprintCallable, Category = "Slide")
	float GetSlideSpeed() const { return SlideSpeed; }

	UFUNCTION(BlueprintCallable, Category = "Slide")
	float GetStandUpSpeed() const { return StandUpSpeed; }

	UFUNCTION(BlueprintCallable, Category = "Slide")
	bool IsSliding() const { return bIsSliding; }

	UFUNCTION(BlueprintCallable, Category = "Slide")
	bool IsStandingUp() const { return bIsStandingUp; }

private:
	UPROPERTY()
	FTimerHandle SlideTimer;

	UPROPERTY()
	float NormalSpeed;

	UPROPERTY()
	float NormalFriction;

	UPROPERTY()
	float NormalDeceleration;

	UPROPERTY()
	float SlideCapsuleHalfHeight;

	UPROPERTY()
	float NormalCapsuleHalfHeight;

	UPROPERTY()
	float CurrentCapsuleHeight;

	UPROPERTY()
	bool bIsSliding = false;

	UPROPERTY()
	bool bIsStandingUp = false;

	// --------- Jump ---------
public:
	UFUNCTION(BlueprintCallable, Category = "Jump")
	void StartJump();

	UFUNCTION(BlueprintCallable, Category = "Jump")
	void StopJump();

protected:
	UFUNCTION(BlueprintCallable, Category = "Jump")
	void InitializeJump();

	// Jump properties
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Abilities")
	bool bCanJump = true;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Jump")
	float JumpZVelocity = 420.0f;

	
	UFUNCTION(BlueprintCallable, Category = "Jump")
	bool CanJump() const { return bCanJump; }

	UFUNCTION(BlueprintCallable, Category = "Jump")
	float GetJumpZVelocity() const { return JumpZVelocity; }

	UFUNCTION(BlueprintCallable, Category = "Jump")
	bool IsJumping() const { return bIsJumping; }

private:
	UPROPERTY()
	bool bIsJumping = false;

	// --------- Sprint ---------
public:
	UFUNCTION(BlueprintCallable, Category = "Sprint")
	void StartSprint();

	UFUNCTION(BlueprintCallable, Category = "Sprint")
	void StopSprint();


	UFUNCTION(BlueprintCallable, Category = "Sprint")
	void SetIsCanSprint(const bool InNewCanSprint) { bCanSprint = InNewCanSprint; }

protected:
	// Sprint properties
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Abilities")
	bool bCanSprint = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sprint")
	float SprintSpeed = 1200.0f;


	UFUNCTION(BlueprintCallable, Category = "Sprint")
	bool CanSprint() const { return bCanSprint; }

	UFUNCTION(BlueprintCallable, Category = "Sprint")
	float GetSprintSpeed() const { return SprintSpeed; }

	UFUNCTION(BlueprintCallable, Category = "Sprint")
	bool IsSprinting() const { return bIsSprinting; }

private:
	UPROPERTY()
	bool bIsSprinting = false;
};
