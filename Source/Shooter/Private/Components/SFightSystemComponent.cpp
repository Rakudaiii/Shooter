#include "Shooter/Public/Components/SFightSystemComponent.h"

#include "GameFramework/Character.h"
#include "Shooter/Public/Guns/SGunBase.h"


// Log category for FightSystemComponent
DEFINE_LOG_CATEGORY(LogFightSystem);


USFightSystemComponent::USFightSystemComponent()
{
	PrimaryComponentTick.bCanEverTick = false;

}


void USFightSystemComponent::BeginPlay()
{
	Super::BeginPlay();

	Initialize();
}


void USFightSystemComponent::Initialize()
{
	OwnerCharacter = Cast<ACharacter>(GetOwner());

	EquipGun(SelectedGun);
}


// ------------ Shoot ------------
void USFightSystemComponent::EquipGun(TSubclassOf<USGunBase> GunClass)
{
	if (!GunClass) return;

	CurrentGun = NewObject<USGunBase>(this, GunClass);

	UE_LOG(LogFightSystem, Warning, TEXT("Current gun name:%s"), *CurrentGun->GetName());
	
	CurrentGun->Init(OwnerCharacter);
}

void USFightSystemComponent::StartFiring()
{
	if (!CurrentGun || !bCanShoot) return;

	CurrentGun->StartFiring();

}

void USFightSystemComponent::StopFiring()
{
	if (!CurrentGun) return;
	
	CurrentGun->StopFiring();
}

void USFightSystemComponent::ReloadGun()
{
	CurrentGun->ReloadAmmo();
}