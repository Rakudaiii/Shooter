#pragma once

#include "CoreMinimal.h"
#include "Shooter/Public/Guns/SGunBase.h"
#include "SAk47.generated.h"


UCLASS()
class SHOOTER_API USAk47 : public USGunBase
{
	GENERATED_BODY()

public:
	virtual void Init(AActor* InActor) override;
	
};
