#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "SParkourMovementComponent.generated.h"

class UCharacterMovementComponent;
class ACharacter;

// Log category for parkour movement
DECLARE_LOG_CATEGORY_EXTERN(LogParkourMovementComponent, Log, All);

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class SHOOTER_API USParkourMovementComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	USParkourMovementComponent();

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	// Core initialization
protected:
	virtual void BeginPlay() override;
	UFUNCTION()
	void Initialize();

	// References to owner and movement component
	UPROPERTY(BlueprintReadOnly, Category = "Component")
	ACharacter* OwnerCharacter;

	UPROPERTY(BlueprintReadOnly, Category = "Component")
	UCharacterMovementComponent* CharacterMovementComponent;

	// --------- Wall Run ---------
public:
	UFUNCTION(BlueprintCallable, Category = "WallRun")
	void LandEvent();

	UFUNCTION(BlueprintCallable, Category = "WallRun")
	void JumpEvent();

protected:
	UFUNCTION(BlueprintCallable, Category = "WallRun")
	void InitializeWallRun();

	UFUNCTION(BlueprintCallable, Category = "WallRun")
	void WallRunUpdate();

	UFUNCTION(BlueprintCallable, Category = "WallRun")
	bool WallRunMovement(const FVector& Start, const FVector& End, float WallRunDirection);

	UFUNCTION(BlueprintCallable, Category = "WallRun")
	void WallRunEnd(float ResetTime);

	UFUNCTION(BlueprintCallable, Category = "WallRun")
	void SuppressWallRun(float Delay);

	UFUNCTION(BlueprintCallable, Category = "WallRun")
	void CameraTick();

	UFUNCTION(BlueprintCallable, Category = "WallRun")
	void CameraTilt(float TargetRollAngle);

	UFUNCTION(BlueprintCallable, Category = "WallRun")
	void WallRunJump();

	UFUNCTION(BlueprintCallable, Category = "WallRun")
	void WallRunUpdateEvent();

	UFUNCTION(BlueprintCallable, Category = "WallRun")
	void ResetWallRunSuppression();

	UFUNCTION(BlueprintCallable, Category = "WallRun")
	void WallRunEndVectors(FVector& OutRight, FVector& OutLeft);

	UFUNCTION(BlueprintCallable, Category = "WallRun")
	bool ValidWallRunVector(const FVector& InVector);

	UFUNCTION(BlueprintCallable, Category = "WallRun")
	FVector PlayerToWallVector();

	// Wall run properties
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Abilities")
	bool bCanWallRun = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "WallRun")
	float WallRunSpeed = 850.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "WallRun")
	float WallRunJumpHeight = 400.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "WallRun")
	float WallRunJumpOffForce = 300.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "WallRun")
	float WallRunTargetGravity = 0.25f;

private:
	UPROPERTY()
	float DefaultGravity;

	UPROPERTY()
	FVector WallRunNormal;

	UPROPERTY()
	FVector Right;

	UPROPERTY()
	FVector Left;

	UPROPERTY()
	FTimerHandle WallRunUpdateTimer;

	UPROPERTY()
	FTimerHandle WallRunSuppressionTimer;

	UPROPERTY()
	bool bWallRunGravity = false;

	UPROPERTY()
	bool bIsWallRunning = false;

	UPROPERTY()
	bool bWallRunningRight = false;

	UPROPERTY()
	bool bWallRunningLeft = false;

	UPROPERTY()
	bool bWallRunSuppressed = false;

	// --------- Dash ---------
public:
	UFUNCTION(BlueprintCallable, Category = "Dash")
	void Dash();

protected:
	UFUNCTION(BlueprintCallable, Category = "Dash")
	void ReloadDash();

	UFUNCTION(BlueprintCallable, Category = "Dash")
	void ReloadingDash();

	// Dash properties
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Abilities")
	bool bCanDash = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dash")
	float DashStrength = 3000.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dash")
	float DashTimeReload = 0.3f;

private:
	UPROPERTY()
	bool bIsDashing = false;

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

private:
	UPROPERTY()
	bool bIsJumping = false;
};