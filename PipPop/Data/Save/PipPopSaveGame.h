// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "PipPopSaveGame.generated.h"

/**
 * 
 */
UCLASS()
class PIPPOP_API UPipPopSaveGame : public USaveGame
{
	GENERATED_BODY()
	
public:
	
	UPROPERTY(BlueprintReadOnly)
	TArray<int32> CharacterMeshes;

	UPROPERTY(BlueprintReadOnly)
	TArray<int32> CharacterMaterials;

	UPipPopSaveGame();
};
