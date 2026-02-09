// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "SaveInterfaces/SaveInterface.h"
#include "PipPopSaveGame.generated.h"

/**
 * 
 */
UCLASS()
class PIPPOP_API UPipPopSaveGame : public USaveGame
{
	GENERATED_BODY()
	
public:
	
	UPipPopSaveGame();

	void SetPlayerID(const FGuid& NewPlayerID);
	
	FGuid GetPlayerID() const;
	
	void SetPlayerSaveData(const TMap<FGuid, FPlayerSaveData>& NewPlayerSaveData);

	TMap<FGuid, FPlayerSaveData> GetPlayerSaveData() const;
	
// private:

	UPROPERTY()
	FGuid PlayerID;
	
	UPROPERTY()
	TArray<int32> CharacterMeshes;
	
	UPROPERTY()
	TArray<int32> CharacterMaterials;

	UPROPERTY()
	TMap<FGuid, FPlayerSaveData> PlayerSave;
	
	
};
