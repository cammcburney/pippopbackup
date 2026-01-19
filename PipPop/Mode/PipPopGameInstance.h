// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "Data/Save/PipPopSaveGame.h"
#include "PipPopGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class PIPPOP_API UPipPopGameInstance : public UGameInstance
{
	GENERATED_BODY()

	UPROPERTY()
	FString SaveName = "Default";

	UPROPERTY()
	TObjectPtr<UPipPopSaveGame> SavedGame;
	
public:

	UPipPopGameInstance();
	
	TObjectPtr<UDataTable> GetAppearanceTable() const {return AppearanceTable;} 

	void SaveCharacterCustomisation(TArray<int32> MeshIndexes);

	void LoadGame();
	
private:
	
	UPROPERTY(EditDefaultsOnly, meta=(RequiredAssetDataTags = "RowStructure=/Script/PipPop.AppearanceInfo"))
	TObjectPtr<UDataTable> AppearanceTable;
};
