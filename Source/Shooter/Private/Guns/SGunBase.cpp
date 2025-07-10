#include "Guns/SGunBase.h"

#include "Camera/CameraComponent.h"
#include "Components/SHealthComponent.h"
#include "Components/SParkourMovementComponent.h"

// Log category for Gun
DEFINE_LOG_CATEGORY(LogGun);


void USGunBase::Init(AActor* InActor)
{
	OwnerActorGun = InActor;
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
	
	if (USParkourMovementComponent* ParkourMovementComp = OwnerActorGun->FindComponentByClass<USParkourMovementComponent>())
		ParkourMovementComp->SetIsCanSprint(true);

	bIsFiring = false;
}	

void USGunBase::Fire()
{
	if (bIsReloading) return;

	if (!OwnerActorGun) return;

	if (USParkourMovementComponent* ParkourMovementComp = OwnerActorGun->FindComponentByClass<USParkourMovementComponent>())
	{
		ParkourMovementComp->StopSprint();
		ParkourMovementComp->SetIsCanSprint(false);
	}

	UCameraComponent* Camera = OwnerActorGun->FindComponentByClass<UCameraComponent>();
	if (!Camera) return;

	FVector Start = Camera->GetComponentLocation();
	FVector End = Start + (Camera->GetForwardVector() * 10000.f);

	FHitResult Hit;
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(OwnerActorGun);

	bool bHit = GetWorld()->LineTraceSingleByChannel(Hit, Start, End, ECC_Visibility, Params);

	if (!bHit || !Hit.GetActor()) return;

	if (USHealthComponent* HealthComp = Hit.GetActor()->FindComponentByClass<USHealthComponent>())
	{
		const float FinalDamage = (Hit.BoneName == "head" || Hit.BoneName == "Head") ? GunDamage * 2.0f : GunDamage;
		HealthComp->TakeDamage(FinalDamage, OwnerActorGun);
	}

	--CurrentAmmo;

	AmmoChangedDelegate.Broadcast(CurrentAmmo);

	if (CurrentAmmo <= 0 && !bIsReloading)
	{
		StopFiring(); 
		ReloadAmmo(); 
	}
}

void USGunBase::ReloadAmmo()
{
	if (bIsReloading || CurrentAmmo >= MaxAmmo)
		return;

	bIsReloading = true;

	GetWorld()->GetTimerManager().SetTimer(ReloadHandle,this,&USGunBase::CompleteReload,ReloadTime,false);

	UE_LOG(LogGun, Log, TEXT("Reload was started"));
}

void USGunBase::CompleteReload()
{
	CurrentAmmo = MaxAmmo;
	
	bIsReloading = false;
	
	UE_LOG(LogGun, Log, TEXT("Reload was completed"));

	AmmoChangedDelegate.Broadcast(CurrentAmmo);
}
