// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/Actors/SwitcherBase.h"
#include "Data/UtilEnums.h"
#include "ItemRotationSwitcher.generated.h"

/**
 * 
 */

UCLASS()
class PIPPOP_API AItemRotationSwitcher : public ASwitcherBase
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere)
	float RotationRate = 5.f;

	UPROPERTY(EditAnywhere)
	EDirections RotationDirection = EDirections::RIGHT;
	
	AItemRotationSwitcher();
	
	virtual void Tick(float DeltaSeconds) override;

	virtual void Interact_Implementation(UPrimitiveComponent* InteractedComponent) override;

private:

	bool bRotate = false;

	UPROPERTY()
	TObjectPtr<UAudioComponent> AudioComponent = nullptr;
};
