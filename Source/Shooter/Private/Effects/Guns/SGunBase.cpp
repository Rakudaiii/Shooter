#include "Shooter/Public/Effects/Guns/SGunBase.h"

#include "GameFramework/Character.h"
#include "Shooter/Public/Components/SFightSystemComponent.h"


DEFINE_LOG_CATEGORY(LogGun);


void USGunBase::ApplyEffect()
{
	Super::ApplyEffect();

	if (USFightSystemComponent* FightSystem = OwnerCharacterGun->FindComponentByClass<USFightSystemComponent>())
	{
		FightSystem->EquipGun(GunClass);
	}
}

void USGunBase::Init(ACharacter* InCharacter)
{
	OwnerCharacterGun = InCharacter;
}

void USGunBase::StartFiring()
{
	
}

void USGunBase::StopFiring()
{
	
}

void USGunBase::Fire()
{
	
}

void USGunBase::ReloadAmmo()
{
	
}

void USGunBase::CompleteReload()
{
	
}