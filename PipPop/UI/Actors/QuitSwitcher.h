// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/Actors/SwitcherBase.h"
#include "QuitSwitcher.generated.h"

/**
 * 
 */
UCLASS()
class PIPPOP_API AQuitSwitcher : public ASwitcherBase
{
	GENERATED_BODY()

public:
	
	virtual void Interact_Implementation(UPrimitiveComponent* InteractedComponent) override;
	
};
