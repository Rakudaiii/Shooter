#include "Shooter/Public/Core/SMainCharacter.h"

#include "Engine/LocalPlayer.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/Controller.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include "Shooter/Public/Components/SBuffManager.h"
#include "Shooter/Public/Components/SFightSystemComponent.h"
#include "Shooter/Public/Components/SHealthComponent.h"
#include "Shooter/Public/Components/SParkourMovementComponent.h"

// Log category for Main Character
DEFINE_LOG_CATEGORY(LogMainCharacter);

ASMainCharacter::ASMainCharacter()
{
	PrimaryActorTick.bCanEverTick = false;

	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);


	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(GetMesh());
	Camera->SetRelativeLocation(FVector(0.0f, 0.0f, 70.0f));
	Camera->bUsePawnControlRotation = true;
	bUseControllerRotationYaw = true;


	HealthComponent = CreateDefaultSubobject<USHealthComponent>(TEXT("HealthComponent"));
	ParkourMovementComponent = CreateDefaultSubobject<USParkourMovementComponent>(TEXT("ParkourMovementComponent"));
	BuffManager = CreateDefaultSubobject<USBuffManager>(TEXT("BuffManager"));
	FightSystemComponent = CreateDefaultSubobject<USFightSystemComponent>(TEXT("FightSystemComponent"));
}


void ASMainCharacter::BeginPlay()
{
	Super::BeginPlay();

	
}

void ASMainCharacter::Landed(const FHitResult& Hit)
{
	Super::Landed(Hit);

	ParkourMovementComponent->LandEvent();
}





void ASMainCharacter::NotifyControllerChanged()
{
	Super::NotifyControllerChanged();

	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<
			UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}
}

void ASMainCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		//Key is Space - Event Jump
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, ParkourMovementComponent,
		                                   &USParkourMovementComponent::StartJump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, ParkourMovementComponent,
		                                   &USParkourMovementComponent::StopJump);

		//Key is Left Shift - Event Dash
		EnhancedInputComponent->BindAction(DashAction, ETriggerEvent::Started, ParkourMovementComponent,
		                                   &USParkourMovementComponent::Dash);

		//Key is C - Event Slide
		EnhancedInputComponent->BindAction(SlideAction, ETriggerEvent::Started, ParkourMovementComponent,
		                                   &USParkourMovementComponent::Slide);

		//Key is Left mouse button - Event Shoot
		/*EnhancedInputComponent->BindAction(ShootAction, ETriggerEvent::Started, FightSystemComponent,
									   &ASMainCharacter::StartFiring);
		EnhancedInputComponent->BindAction(ShootAction, ETriggerEvent::Completed, FightSystemComponent,
									   &ASMainCharacter::StopFiring);

		//Key is R - Reload Ammo
		EnhancedInputComponent->BindAction(ReloadAction, ETriggerEvent::Started, FightSystemComponent,
									   &ASMainCharacter::ReloadGun);*/

		//Keys WASD - Event Move
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ASMainCharacter::Move);

		//Mouse X&Y - Event Look
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &ASMainCharacter::Look);
	}
}

void ASMainCharacter::Move(const FInputActionValue& Value)
{
	FVector2D MovementVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);

		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		AddMovementInput(ForwardDirection, MovementVector.Y);
		AddMovementInput(RightDirection, MovementVector.X);
	}
}

void ASMainCharacter::Look(const FInputActionValue& Value)
{
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);
	}
}
