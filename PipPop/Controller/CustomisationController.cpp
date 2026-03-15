// Fill out your copyright notice in the Description page of Project Settings.


#include "Controller/CustomisationController.h"
#include "Character/MenuPawn.h"

void ACustomisationController::OnPossess(APawn* NewPawn)
{
	Super::OnPossess(NewPawn);
	FInputModeGameAndUI GameInput;
	SetInputMode(GameInput);
	SetShowMouseCursor(true);
}