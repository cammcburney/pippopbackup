// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "Data/Save/PipPopSaveGame.h"
#include "Interfaces/OnlineSessionDelegates.h"
#include "OnlineSessionSettings.h"
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
	
	TObjectPtr<UDataTable> GetLevelTable() const {return LevelTable;}
	
	void SaveCharacterCustomisation(TArray<int32> MeshIndexes);

	void LoadGame();

	void HostSession(const FName& SessionName);

	void FindSessions();

	void FindSessionsComplete(bool bSearchCompleted);

	FOnFindSessionsCompleteDelegate FindSessionsCompleteDelegate;

	FDelegateHandle FindSessionsCompleteHandle;
	
	TSharedPtr<FOnlineSessionSettings> SessionSettings;
	
	TSharedPtr<FOnlineSessionSearch> SearchSettings;
	
	void LoadLevel(const FName LevelName);

private:
	
	UPROPERTY(EditDefaultsOnly, meta=(RequiredAssetDataTags = "RowStructure=/Script/PipPop.AppearanceInfo"))
	TObjectPtr<UDataTable> AppearanceTable;

	UPROPERTY(EditDefaultsOnly, meta=(RequiredAssetDataTags = "RowStructure=/Script/PipPop.LevelNames"))
	TObjectPtr<UDataTable> LevelTable;
};
