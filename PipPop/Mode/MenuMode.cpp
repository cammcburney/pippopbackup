// Fill out your copyright notice in the Description page of Project Settings.


#include "MenuMode.h"
#include "Kismet/GameplayStatics.h"

void AMenuMode::BeginPlay()
{
	Super::BeginPlay();
	if (APlayerController* Controller = UGameplayStatics::GetPlayerController(this, 0))
	{
		Controller->SetShowMouseCursor(true);
	}
}
