#include "Guns/SGunBase.h"

#include "Camera/CameraComponent.h"
#include "Components/SHealthComponent.h"
#include "Components/SParkourMovementComponent.h"
#include "GameFramework/Character.h"

// Log category for Gun
DEFINE_LOG_CATEGORY(LogGun);


void USGunBase::Init(ACharacter* InCharacter)
{
	OwnerCharacterGun = InCharacter;
}

void USGunBase::StartFiring()
{
	if (CurrentAmmo > 0 && !bIsReloading)
	{
		Fire();
		bIsFiring = true;
		
		if (bIsAuto)
		{
			GetWorld()->GetTimerManager().SetTimer(AutoFireHandle, this, &USGunBase::Fire, FireRate, true);
		}
	}
	else
	{
		ReloadAmmo();
	}
}

void USGunBase::StopFiring()
{
	if (bIsAuto)
		GetWorld()->GetTimerManager().ClearTimer(AutoFireHandle);
	
	if (USParkourMovementComponent* ParkourMovementComp = OwnerCharacterGun->FindComponentByClass<USParkourMovementComponent>())
		ParkourMovementComp->SetIsCanSprint(true);

	bIsFiring = false;
}	

void USGunBase::Fire()
{
	if (bIsReloading) return;

	if (!OwnerCharacterGun) return;

	if (USParkourMovementComponent* ParkourMovementComp = OwnerCharacterGun->FindComponentByClass<USParkourMovementComponent>())
	{
		ParkourMovementComp->StopSprint();
		ParkourMovementComp->SetIsCanSprint(false);
	}

	UCameraComponent* Camera = OwnerCharacterGun->FindComponentByClass<UCameraComponent>();
	if (!Camera) return;

	FVector Start = Camera->GetComponentLocation();
	FVector End = Start + (Camera->GetForwardVector() * 10000.f);

	FHitResult Hit;
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(OwnerCharacterGun);

	bool bHit = GetWorld()->LineTraceSingleByChannel(Hit, Start, End, ECC_Visibility, Params);

	if (!bHit || !Hit.GetActor()) return;

	if (USHealthComponent* HealthComp = Hit.GetActor()->FindComponentByClass<USHealthComponent>())
	{
		const float FinalDamage = (Hit.BoneName == "head" || Hit.BoneName == "Head") ? GunDamage * 2.0f : GunDamage;
		HealthComp->TakeDamage(FinalDamage, OwnerCharacterGun);
	}

	--CurrentAmmo;

	if (CurrentAmmo <= 0 && !bIsReloading)
	{
		StopFiring(); 
		ReloadAmmo(); 
	}
	
	DrawDebugLine(GetWorld(), Start, End, FColor::Red, false, 2.0f);
}

void USGunBase::ReloadAmmo()
{
	if (bIsReloading || CurrentAmmo >= MaxAmmo)
		return;


	bIsReloading = true;

	GetWorld()->GetTimerManager().SetTimer(
		ReloadHandle,
		this,
		&USGunBase::CompleteReload,
		ReloadTime,
		false
	);

	UE_LOG(LogGun, Log, TEXT("Start Reload"));
}

void USGunBase::CompleteReload()
{
	CurrentAmmo = MaxAmmo;
	bIsReloading = false;
	UE_LOG(LogGun, Log, TEXT("Reload complete"));
}
