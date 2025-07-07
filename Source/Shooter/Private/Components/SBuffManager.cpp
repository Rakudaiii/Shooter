#include "Shooter/Public/Components/SBuffManager.h"

#include "GameFramework/Character.h"
#include "Shooter/Public/Effects/SEffect.h"
#include "Shooter/Public/Effects/SEffectBase.h"


USBuffManager::USBuffManager()
{
	PrimaryComponentTick.bCanEverTick = false;
}


void USBuffManager::BeginPlay()
{
	Super::BeginPlay();

	Initialize();
}


void USBuffManager::Initialize()
{
	OwnerCharacter = Cast<ACharacter>(GetOwner());
}

void USBuffManager::ApplyEffect(TSubclassOf<USEffect> Effect)
{
	if (!Effect) return;

	USEffectBase* NewEffect = NewObject<USEffectBase>(this, Effect);
	if (!NewEffect) return;

	NewEffect->Initialize(OwnerCharacter);
	NewEffect->ApplyEffect();
	
}


