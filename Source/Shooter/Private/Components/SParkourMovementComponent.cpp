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
	
	InitializeSlide();
	InitializeJump();
}


void USParkourMovementComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                               FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	StandUpAnimation(DeltaTime);
}

// --------- Slide ---------
void USParkourMovementComponent::InitializeSlide()
{
	if (!bCanSlide) return;

	
	NormalSpeed = CharacterMovementComponent->MaxWalkSpeed;
	NormalFriction = CharacterMovementComponent->GroundFriction;
	NormalDeceleration = CharacterMovementComponent->BrakingDecelerationWalking;
	NormalCapsuleHalfHeight = OwnerCharacter->GetCapsuleComponent()->GetScaledCapsuleHalfHeight();
	SlideCapsuleHalfHeight = NormalCapsuleHalfHeight / 2.0f;
	
	UE_LOG(LogParkourMovementComponent, Log, TEXT("Slide initialized successfully."));
}

void USParkourMovementComponent::Slide()
{
	if (!CharacterMovementComponent || bIsSliding || CharacterMovementComponent->IsFalling() || !bCanSlide || OwnerCharacter->GetVelocity().Length() < 10.0f)
		return;

	UE_LOG(LogParkourMovementComponent, Warning, TEXT("Sliding"));

	if (bIsStandingUp)
	{
		OwnerCharacter->GetCapsuleComponent()->SetCapsuleHalfHeight(NormalCapsuleHalfHeight);
		CurrentCapsuleHeight = NormalCapsuleHalfHeight;
		bIsStandingUp = false;
		UE_LOG(LogParkourMovementComponent, Log, TEXT("Forced stand-up completion before sliding"));
	}
	
	bIsSliding = true;

	StopSprint();
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


	GetWorld()->GetTimerManager().SetTimer(SlideTimer, this, &USParkourMovementComponent::StopSlide, SlideDuration,false);
}

void USParkourMovementComponent::StopSlide()
{
	CharacterMovementComponent->MaxWalkSpeed = NormalSpeed;
	CharacterMovementComponent->GroundFriction = NormalFriction;
	CharacterMovementComponent->BrakingDecelerationWalking = NormalDeceleration;
	bIsSliding = false;

	CurrentCapsuleHeight = SlideCapsuleHalfHeight;
	bIsStandingUp = true;


	GetWorld()->GetTimerManager().ClearTimer(SlideTimer);

	UE_LOG(LogParkourMovementComponent, Warning, TEXT("Slide stopped"));
}

void USParkourMovementComponent::StandUpAnimation(float DeltaTime)
{
	if (bIsStandingUp)
	{
		UCapsuleComponent* Capsule = OwnerCharacter->GetCapsuleComponent();
		if (Capsule)
		{
			CurrentCapsuleHeight = FMath::FInterpTo(CurrentCapsuleHeight, NormalCapsuleHalfHeight, DeltaTime,
			                                        StandUpSpeed);
			Capsule->SetCapsuleHalfHeight(CurrentCapsuleHeight);

			if (FMath::IsNearlyEqual(CurrentCapsuleHeight, NormalCapsuleHalfHeight, 0.1f))
			{
				Capsule->SetCapsuleHalfHeight(NormalCapsuleHalfHeight);
				bIsStandingUp = false;
				UE_LOG(LogParkourMovementComponent, Log, TEXT("Stand-up animation completed. CurrentCapsuleHeight: %f"),
				       CurrentCapsuleHeight);
			}
		}
	}
}

// --------- Jump ---------
void USParkourMovementComponent::InitializeJump()
{
	if (!bCanJump) return;

	CharacterMovementComponent->JumpZVelocity = JumpZVelocity;
}

void USParkourMovementComponent::StartJump()
{
	if (!bCanJump) return;


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

	StopSprint();
	UE_LOG(LogParkourMovementComponent, Warning, TEXT("Jump"));
	OwnerCharacter->Jump();
	bIsJumping = true;
}

void USParkourMovementComponent::StopJump()
{
	if (!bCanJump) return;

	OwnerCharacter->StopJumping();
	bIsJumping = false;

	UE_LOG(LogParkourMovementComponent, Warning, TEXT("Jump stopped"));
}

// --------- Sprint ---------
void USParkourMovementComponent::StartSprint()
{
	if (!CharacterMovementComponent || bIsSprinting || bIsSliding || CharacterMovementComponent->IsFalling() || !bCanSprint)
		return;

	UE_LOG(LogParkourMovementComponent, Warning, TEXT("Sprinting"));
	bIsSprinting = true;
	CharacterMovementComponent->MaxWalkSpeed = SprintSpeed;
}

void USParkourMovementComponent::StopSprint()
{
	if (!bIsSprinting) return;

	CharacterMovementComponent->MaxWalkSpeed = NormalSpeed;
	bIsSprinting = false;
	UE_LOG(LogParkourMovementComponent, Warning, TEXT("Sprint stopped"));
}
