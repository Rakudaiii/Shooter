#include "Shooter/Public/Components/SParkourMovementComponent.h"

#include "Components/CapsuleComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"

// Log category definition
DEFINE_LOG_CATEGORY(LogParkourMovementComponent);

USParkourMovementComponent::USParkourMovementComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void USParkourMovementComponent::BeginPlay()
{
	Super::BeginPlay();
	Initialize();
}

void USParkourMovementComponent::Initialize()
{
	OwnerCharacter = Cast<ACharacter>(GetOwner());
	CharacterMovementComponent = OwnerCharacter->GetCharacterMovement();
	

	InitializeWallRun();
	InitializeSlide();
	InitializeJump();
}

void USParkourMovementComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	CameraTick();
	StandUpAnimation(DeltaTime);
}



// --------- Wall Run ---------
void USParkourMovementComponent::JumpEvent()
{
	WallRunJump();
}

void USParkourMovementComponent::LandEvent()
{
	WallRunEnd(0.0f);
	bWallRunSuppressed = false;
}

void USParkourMovementComponent::InitializeWallRun()
{
	if (!bCanWallRun)
	{
		UE_LOG(LogParkourMovementComponent, Log, TEXT("Wall Run initialization skipped: bCanWallRun is false."));
		return;
	}

	UE_LOG(LogParkourMovementComponent, Log, TEXT("Wall Run initialized successfully."));
	DefaultGravity = CharacterMovementComponent->GravityScale;

	GetWorld()->GetTimerManager().SetTimer(WallRunUpdateTimer, this, &USParkourMovementComponent::WallRunUpdateEvent, 0.2f, true);
}

void USParkourMovementComponent::WallRunUpdate()
{
	if (!bCanWallRun || bWallRunSuppressed)
	{
		return;
	}

	FVector Start = OwnerCharacter->GetActorLocation();
	WallRunEndVectors(Right, Left);

	if (WallRunMovement(Start, Right, -1.0f))
	{
		bIsWallRunning = true;
		bWallRunningRight = true;
		bWallRunningLeft = false;

		CharacterMovementComponent->GravityScale = FMath::FInterpTo(CharacterMovementComponent->GravityScale, WallRunTargetGravity, GetWorld()->GetDeltaSeconds(), 10.0f);
	}
	else if (WallRunMovement(Start, Left, 1.0f))
	{
		bIsWallRunning = true;
		bWallRunningRight = false;
		bWallRunningLeft = true;

		CharacterMovementComponent->GravityScale = FMath::FInterpTo(CharacterMovementComponent->GravityScale, WallRunTargetGravity, GetWorld()->GetDeltaSeconds(), 10.0f);
	}
	else
	{
		WallRunEnd(1.0f);
	}
}

bool USParkourMovementComponent::WallRunMovement(const FVector& Start, const FVector& End, float WallRunDirection)
{
	FHitResult Hit;
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(GetOwner());

	if (GetWorld()->LineTraceSingleByChannel(Hit, Start, End, ECC_Visibility, Params) && Hit.bBlockingHit)
	{
		if (ValidWallRunVector(Hit.Normal) && CharacterMovementComponent->IsFalling())
		{
			WallRunNormal = Hit.Normal;
			OwnerCharacter->LaunchCharacter(PlayerToWallVector(), false, false);

			FVector LaunchVector = WallRunNormal.Cross(FVector(0.0f, 0.0f, 1.0f)) * WallRunDirection * WallRunSpeed;
			OwnerCharacter->LaunchCharacter(LaunchVector, true, !bWallRunGravity);
			return true;
		}
	}
	return false;
}

void USParkourMovementComponent::WallRunEnd(float ResetTime)
{
	if (bIsWallRunning)
	{
		bIsWallRunning = false;
		bWallRunningRight = false;
		bWallRunningLeft = false;
		CharacterMovementComponent->GravityScale = DefaultGravity;
		SuppressWallRun(ResetTime);
	}
}

void USParkourMovementComponent::SuppressWallRun(float Delay)
{
	bWallRunSuppressed = true;
	GetWorld()->GetTimerManager().SetTimer(WallRunSuppressionTimer, this, &USParkourMovementComponent::ResetWallRunSuppression, Delay, false);
}

void USParkourMovementComponent::CameraTick()
{
	const float TiltAngle = bWallRunningLeft ? 15.0f : (bWallRunningRight ? -15.0f : 0.0f);
	CameraTilt(TiltAngle);
}

void USParkourMovementComponent::CameraTilt(float TargetRollAngle)
{
	if (OwnerCharacter->GetController())
	{
		FRotator CurrentRotation = OwnerCharacter->GetController()->GetControlRotation();
		FRotator TargetRotation(CurrentRotation.Pitch, CurrentRotation.Yaw, TargetRollAngle);
		OwnerCharacter->GetController()->SetControlRotation(FMath::RInterpTo(CurrentRotation, TargetRotation, GetWorld()->GetDeltaSeconds(), 10.0f));
	}
}

void USParkourMovementComponent::WallRunJump()
{
	if (bIsWallRunning)
	{
		WallRunEnd(0.35f);
		FVector LaunchVector(WallRunNormal.X * WallRunJumpOffForce, WallRunNormal.Y * WallRunJumpOffForce, WallRunJumpHeight);
		OwnerCharacter->LaunchCharacter(LaunchVector, false, true);
	}
}

void USParkourMovementComponent::WallRunUpdateEvent()
{
	if (!bWallRunSuppressed)
	{
		WallRunUpdate();
	}
}

void USParkourMovementComponent::ResetWallRunSuppression()
{
	GetWorld()->GetTimerManager().ClearTimer(WallRunSuppressionTimer);
	bWallRunSuppressed = false;
}

void USParkourMovementComponent::WallRunEndVectors(FVector& OutRight, FVector& OutLeft)
{
	FVector StartLocation = OwnerCharacter->GetActorLocation();
	FVector RightDir = OwnerCharacter->GetActorRightVector();
	FVector ForwardDir = OwnerCharacter->GetActorForwardVector();

	OutRight = StartLocation + (RightDir * 75.0f) + (ForwardDir * -35.0f);
	OutLeft = StartLocation + (RightDir * -75.0f) + (ForwardDir * -35.0f);
}

bool USParkourMovementComponent::ValidWallRunVector(const FVector& InVector)
{
	return FMath::IsWithinInclusive(InVector.Z, -0.52f, 0.52f);
}

FVector USParkourMovementComponent::PlayerToWallVector()
{
	FVector WallAttractionVector = OwnerCharacter->GetActorLocation() - WallRunNormal;
	return WallRunNormal * WallAttractionVector.Length();
}

// --------- Dash ---------
void USParkourMovementComponent::Dash()
{
	if (!CharacterMovementComponent || !bCanDash || bIsDashing || OwnerCharacter->GetVelocity().Length() < 10.0f)
	{
		return;
	}

	UE_LOG(LogParkourMovementComponent, Warning, TEXT("Dashing"));
	bIsDashing = true;

	float EffectiveDashStrength = CharacterMovementComponent->IsFalling() ? DashStrength * 0.3f : DashStrength;
	FVector DashInput = OwnerCharacter->GetLastMovementInputVector().IsNearlyZero() ? OwnerCharacter->GetActorForwardVector() : OwnerCharacter->GetLastMovementInputVector();
	DashInput.Z = 0.0f;
	DashInput.Normalize();

	FVector DashVelocity = DashInput * EffectiveDashStrength;
	OwnerCharacter->LaunchCharacter(DashVelocity.GetClampedToMaxSize(EffectiveDashStrength), true, false);

	ReloadDash();
}

void USParkourMovementComponent::ReloadDash()
{
	GetWorld()->GetTimerManager().SetTimerForNextTick(this, &USParkourMovementComponent::ReloadingDash);
}

void USParkourMovementComponent::ReloadingDash()
{
	bIsDashing = false;
}

// --------- Slide ---------
void USParkourMovementComponent::InitializeSlide()
{
	if (!bCanSlide)
	{
		UE_LOG(LogParkourMovementComponent, Log, TEXT("Slide initialization skipped: bCanSlide is false."));
		return;
	}

	UE_LOG(LogParkourMovementComponent, Log, TEXT("Slide initialized successfully."));
	NormalSpeed = CharacterMovementComponent->MaxWalkSpeed;
	NormalFriction = CharacterMovementComponent->GroundFriction;
	NormalDeceleration = CharacterMovementComponent->BrakingDecelerationWalking;
	NormalCapsuleHalfHeight = OwnerCharacter->GetCapsuleComponent()->GetScaledCapsuleHalfHeight();
	SlideCapsuleHalfHeight = NormalCapsuleHalfHeight / 2.0f;
}

void USParkourMovementComponent::Slide()
{
	if (!CharacterMovementComponent || bIsSliding || CharacterMovementComponent->IsFalling() || !bCanSlide || OwnerCharacter->GetVelocity().Length() < 10.0f)
	{
		UE_LOG(LogParkourMovementComponent, Warning, TEXT("Slide blocked: bIsSliding=%d, IsFalling=%d, bCanSlide=%d, Velocity=%f"),
			bIsSliding, CharacterMovementComponent->IsFalling(), bCanSlide, OwnerCharacter->GetVelocity().Length());
		return;
	}

	if (bIsStandingUp)
	{
		UCapsuleComponent* Capsule = OwnerCharacter->GetCapsuleComponent();
		if (Capsule)
		{
			Capsule->SetCapsuleHalfHeight(NormalCapsuleHalfHeight);
			bIsStandingUp = false;
			UE_LOG(LogParkourMovementComponent, Log, TEXT("Stand-up animation interrupted for new slide."));
		}
	}

	UE_LOG(LogParkourMovementComponent, Warning, TEXT("Sliding"));
	bIsSliding = true;
	StartSlide();
}

void USParkourMovementComponent::StartSlide()
{
	CharacterMovementComponent->MaxWalkSpeed = SlideSpeed;
	CharacterMovementComponent->GroundFriction = SlideFriction;
	CharacterMovementComponent->BrakingDecelerationWalking = SlideDeceleration;
	OwnerCharacter->GetCapsuleComponent()->SetCapsuleHalfHeight(SlideCapsuleHalfHeight);

	FVector LaunchVelocity = OwnerCharacter->GetLastMovementInputVector() * SlideSpeed;
	OwnerCharacter->LaunchCharacter(LaunchVelocity, false, false);

	if (GetWorld())
	{
		GetWorld()->GetTimerManager().SetTimer(SlideTimer, this, &USParkourMovementComponent::StopSlide, SlideDuration, false);
	}
	else
	{
		UE_LOG(LogParkourMovementComponent, Error, TEXT("GetWorld() is null in StartSlide."));
	}
}

void USParkourMovementComponent::StopSlide()
{
	CharacterMovementComponent->MaxWalkSpeed = NormalSpeed;
	CharacterMovementComponent->GroundFriction = NormalFriction;
	CharacterMovementComponent->BrakingDecelerationWalking = NormalDeceleration;
	bIsSliding = false;

	CurrentCapsuleHeight = SlideCapsuleHalfHeight;
	bIsStandingUp = true;

	if (GetWorld())
	{
		GetWorld()->GetTimerManager().ClearTimer(SlideTimer);
	}
}

void USParkourMovementComponent::StandUpAnimation(float DeltaTime)
{
	if (bIsStandingUp)
	{
		UCapsuleComponent* Capsule = OwnerCharacter->GetCapsuleComponent();
		if (Capsule)
		{
			CurrentCapsuleHeight = FMath::FInterpTo(CurrentCapsuleHeight, NormalCapsuleHalfHeight, DeltaTime, StandUpSpeed);
			Capsule->SetCapsuleHalfHeight(CurrentCapsuleHeight);

			if (FMath::IsNearlyEqual(CurrentCapsuleHeight, NormalCapsuleHalfHeight, 0.1f))
			{
				Capsule->SetCapsuleHalfHeight(NormalCapsuleHalfHeight);
				bIsStandingUp = false;
				UE_LOG(LogParkourMovementComponent, Log, TEXT("Stand-up animation completed. CurrentCapsuleHeight: %f"), CurrentCapsuleHeight);
			}
		}
	}
}

// --------- Jump ---------
void USParkourMovementComponent::InitializeJump()
{
	if (bCanJump)
	{
		CharacterMovementComponent->JumpZVelocity = JumpZVelocity;
	}
}

void USParkourMovementComponent::StartJump()
{
	if (!bCanJump)
	{
		return;
	}

	// Прерываем слайд или вставание, если они активны
	if (bIsSliding || bIsStandingUp)
	{
		StopSlide();
		UCapsuleComponent* Capsule = OwnerCharacter->GetCapsuleComponent();
		if (Capsule)
		{
			Capsule->SetCapsuleHalfHeight(NormalCapsuleHalfHeight);
			bIsStandingUp = false;
			UE_LOG(LogParkourMovementComponent, Log, TEXT("Slide or stand-up interrupted for jump."));
		}
	}

	UE_LOG(LogParkourMovementComponent, Warning, TEXT("Jump"));
	OwnerCharacter->Jump();
	JumpEvent();
	bIsJumping = true;
}

void USParkourMovementComponent::StopJump()
{
	if (bCanJump)
	{
		OwnerCharacter->StopJumping();
		bIsJumping = false;
	}
}