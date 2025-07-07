#include "Shooter/Public/Effects/SEffectBase.h"

#include "GameFramework/Character.h"




void USEffectBase::Initialize(AActor* Actor)
{
	OwnerCharacter = Cast<ACharacter>(Actor);
}


void USEffectBase::ApplyEffect()
{
	
}