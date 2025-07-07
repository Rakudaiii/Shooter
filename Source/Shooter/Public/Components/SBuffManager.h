#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "SBuffManager.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class SHOOTER_API USBuffManager : public UActorComponent
{
	GENERATED_BODY()

public:
	USBuffManager();

	UFUNCTION(BlueprintCallable)
	void Initialize();

	UFUNCTION(BlueprintCallable, Category = "Array")
	void ApplyEffect(TSubclassOf<USEffect> Effect);

protected:
	UPROPERTY()
	ACharacter* OwnerCharacter;

	virtual void BeginPlay() override;
	
	
};



