// Fill out your copyright notice in the Description page of Project Settings.


#include "Mode/Menu/CreationGameMode.h"
#include "Kismet/GameplayStatics.h"

void ACreationGameMode::BeginPlay()
{
	Super::BeginPlay();
	if (APlayerController* Controller = UGameplayStatics::GetPlayerController(this, 0))
	{
		Controller->SetShowMouseCursor(true);
	}
}
