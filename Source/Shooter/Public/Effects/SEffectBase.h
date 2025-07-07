#pragma once

#include "CoreMinimal.h"
#include "SEffect.h"
#include "SEffectBase.generated.h"

UCLASS()
class SHOOTER_API USEffectBase : public USEffect
{
	GENERATED_BODY()

	
public:
	UFUNCTION(BlueprintCallable)
	void Initialize(AActor* Actor);


	UFUNCTION(BlueprintCallable)
	virtual void ApplyEffect();

protected:

	UPROPERTY()
	ACharacter* OwnerCharacter;

};
