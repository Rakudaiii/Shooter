#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "SEnemy.generated.h"

UCLASS()
class SHOOTER_API ASEnemy : public ACharacter
{
	GENERATED_BODY()

public:
	ASEnemy();

protected:
	virtual void BeginPlay() override;


};
