#include "Shooter/Public/Components/SFightSystemComponent.h"

#include "GameFramework/Character.h"
#include "Shooter/Public/Effects/Guns/SGunBase.h"


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
	
}


// ------------ Shoot ------------
void USFightSystemComponent::EquipGun(TSubclassOf<USGunBase> GunClass)
{
	if (!GunClass) return;

	CurrentGun = NewObject<USGunBase>(this, GunClass);
	
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