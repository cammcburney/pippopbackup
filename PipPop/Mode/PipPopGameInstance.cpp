// Fill out your copyright notice in the Description page of Project Settings.


#include "Mode/PipPopGameInstance.h"

#include "Data/Save/PipPopSaveGame.h"
#include "Kismet/GameplayStatics.h"

UPipPopGameInstance::UPipPopGameInstance()
{
	SavedGame = Cast<UPipPopSaveGame>(UGameplayStatics::LoadGameFromSlot(SaveName, 0));
	if (!SavedGame)
	{
		SavedGame = Cast<UPipPopSaveGame>(UGameplayStatics::CreateSaveGameObject(UPipPopSaveGame::StaticClass()));
	}
}

void UPipPopGameInstance::SaveCharacterCustomisation(TArray<int32> MeshIndexes)
{
	SavedGame->CharacterMeshes = MeshIndexes;
}

