#include "Shooter/Public/Components/SWeaponManager.h"

#include "GameFramework/Character.h"
#include "Shooter/Public/Guns/SGunBase.h"


// Log category for FightSystemComponent
DEFINE_LOG_CATEGORY(LogWeaponManager);


USWeaponManager::USWeaponManager()
{
	PrimaryComponentTick.bCanEverTick = false;

}


void USWeaponManager::BeginPlay()
{
	Super::BeginPlay();

	Initialize();
}


void USWeaponManager::Initialize()
{
	OwnerActor = GetOwner();

	EquipGun(SelectedGun);
}


// ------------ Shoot ------------
void USWeaponManager::EquipGun(TSubclassOf<USGunBase> GunClass)
{
	if (!GunClass) return;

	CurrentGun = NewObject<USGunBase>(this, GunClass);

	UE_LOG(LogWeaponManager, Log, TEXT("Current gun name:%s"), *CurrentGun->GetName());
	
	CurrentGun->Init(OwnerActor);
}

void USWeaponManager::StartFiring()
{
	if (!CurrentGun || !bCanShoot) return;
	
	CurrentGun->StartFiring();

	UE_LOG(LogWeaponManager, Warning, TEXT("Start Firing"));
}

void USWeaponManager::StopFiring()
{
	if (!CurrentGun) return;
	
	CurrentGun->StopFiring();
	
	UE_LOG(LogWeaponManager, Warning, TEXT("Fire stopped"));
}

void USWeaponManager::ReloadGun()
{
	CurrentGun->ReloadAmmo();
	
	UE_LOG(LogWeaponManager, Warning, TEXT("Start Reloading"));
}