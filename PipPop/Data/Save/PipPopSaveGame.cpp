// Fill out your copyright notice in the Description page of Project Settings.


#include "PipPopSaveGame.h"
#include "SaveInterfaces/SaveInterface.h"

UPipPopSaveGame::UPipPopSaveGame()
{
	
}

void UPipPopSaveGame::SetPlayerID(const FGuid& NewPlayerID)
{
	PlayerID = NewPlayerID;
}

FGuid UPipPopSaveGame::GetPlayerID() const
{
	return PlayerID;
}

void UPipPopSaveGame::SetPlayerSaveData(const TMap<FGuid, FPlayerSaveData>& NewPlayerSaveData)
{
	PlayerSave = NewPlayerSaveData;
}

TMap<FGuid, FPlayerSaveData> UPipPopSaveGame::GetPlayerSaveData() const
{
	return PlayerSave;
}
