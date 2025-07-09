#include "Shooter/Public/Core/SMainCharacter.h"

#include "Engine/LocalPlayer.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/Controller.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Shooter/Public/Components/SWeaponManager.h"
#include "Shooter/Public/Components/SHealthComponent.h"
#include "Shooter/Public/Components/SParkourMovementComponent.h"

// Log category for Main Character
DEFINE_LOG_CATEGORY(LogMainCharacter);


ASMainCharacter::ASMainCharacter()
{
	PrimaryActorTick.bCanEverTick = false;


	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);


	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(GetCapsuleComponent());
	Camera->bUsePawnControlRotation = true;

	GetMesh()->SetupAttachment(Camera);
	GetMesh()->CastShadow = false;


	bUseControllerRotationYaw = false;
	GetCharacterMovement()->bOrientRotationToMovement = true;


	HealthComponent = CreateDefaultSubobject<USHealthComponent>(TEXT("HealthComponent"));
	ParkourMovementComponent = CreateDefaultSubobject<USParkourMovementComponent>(TEXT("ParkourMovementComponent"));
	WeaponManager = CreateDefaultSubobject<USWeaponManager>(TEXT("FightSystemComponent"));
}

void ASMainCharacter::BeginPlay()
{
	Super::BeginPlay();
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

		//Key is Left Shift - Event Sprint
		EnhancedInputComponent->BindAction(SprintAction, ETriggerEvent::Started, ParkourMovementComponent,
		                                   &USParkourMovementComponent::StartSprint);
		EnhancedInputComponent->BindAction(SprintAction, ETriggerEvent::Completed, ParkourMovementComponent,
		                                   &USParkourMovementComponent::StopSprint);

		//Key is C - Event Slide
		EnhancedInputComponent->BindAction(SlideAction, ETriggerEvent::Started, ParkourMovementComponent,
		                                   &USParkourMovementComponent::Slide);

		//Key is Left mouse button - Event Shoot
		EnhancedInputComponent->BindAction(ShootAction, ETriggerEvent::Started, WeaponManager,
		                                   &USWeaponManager::StartFiring);
		EnhancedInputComponent->BindAction(ShootAction, ETriggerEvent::Completed, WeaponManager,
		                                   &USWeaponManager::StopFiring);

		//Key is R - Reload Ammo
		EnhancedInputComponent->BindAction(ReloadAction, ETriggerEvent::Started, WeaponManager,
		                                   &USWeaponManager::ReloadGun);

		//Keys WASD - Event Move
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ASMainCharacter::Move);

		//Mouse X&Y - Event Look
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &ASMainCharacter::Look);
	}
}

void ASMainCharacter::Move(const FInputActionValue& Value)
{
	FVector2D MovementVector = Value.Get<FVector2D>();

	if (!Controller) return;

	const FRotator Rotation = Controller->GetControlRotation();
	const FRotator YawRotation(0, Rotation.Yaw, 0);

	const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);

	const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

	AddMovementInput(ForwardDirection, MovementVector.Y);
	AddMovementInput(RightDirection, MovementVector.X);
}

void ASMainCharacter::Look(const FInputActionValue& Value)
{
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (!Controller) return;

	AddControllerYawInput(LookAxisVector.X);
	AddControllerPitchInput(LookAxisVector.Y);
}
