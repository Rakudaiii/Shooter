#include "Shooter/Public/Guns/Rifles/SAk47.h"

void USAk47::Init(AActor* InActor)
{
	Super::Init(InActor);

	bIsAuto = true;
	FireRate = 0.3f;
	GunDamage = 20.0f;

	ReloadTime = 2.0f;
	MaxAmmo = 30;
	CurrentAmmo = MaxAmmo;
}
