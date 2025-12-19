// Fill out your copyright notice in the Description page of Project Settings.


#include "Mode/CreationGameMode.h"

#include "Blueprint/UserWidget.h"
#include "Widgets/CustomisationManager.h"
#include "Kismet/GameplayStatics.h"

void ACreationGameMode::BeginPlay()
{
	Super::BeginPlay();
	if (APlayerController* Controller = UGameplayStatics::GetPlayerController(this, 0))
	{
		Controller->SetShowMouseCursor(true);
		CustomisationManager = CreateWidget<UCustomisationManager>(Controller, WidgetManagerClass);
		if (CustomisationManager)
		{
			CustomisationManager->SetOwningPlayer(Controller);
			CustomisationManager->AddToViewport();
			CustomisationManager->Init();
		}
	}
}
