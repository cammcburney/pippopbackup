// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Actors/QuitSwitcher.h"
#include "Kismet/GameplayStatics.h"
void AQuitSwitcher::Interact_Implementation(UPrimitiveComponent* InteractedComponent)
{
	if (APlayerController* PlayerController = GetWorld()->GetFirstPlayerController())
	{
		UKismetSystemLibrary::QuitGame(GetWorld(), PlayerController, EQuitPreference::Quit, false);
	}
}
