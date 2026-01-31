// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SwitcherBase.h"
#include "Data/LevelStructs.h"
#include "LevelSwitcher.generated.h"

class UUserWidget;

UCLASS()
class PIPPOP_API ALevelSwitcher : public ASwitcherBase
{
	GENERATED_BODY()
	
public:	

	UPROPERTY(EditInstanceOnly)
	TSubclassOf<UUserWidget> TransitionWidgetClass;

	UPROPERTY(EditInstanceOnly)
	ELevelStructs LevelReference;
	
	virtual void Interact_Implementation(UPrimitiveComponent* InteractedComponent) override;

};
