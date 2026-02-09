// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "SaveInterface.generated.h"

USTRUCT(BlueprintType)
struct FPlayerSaveData
{
	GENERATED_USTRUCT_BODY()
	
	FName GetPlayerName() const {return PlayerName;}

	void SetPlayerName(const FName& NewPlayerName) {PlayerName = NewPlayerName;}

private:

	UPROPERTY()
	FName PlayerName = FName(NAME_None);

	UPROPERTY()
	TArray<uint8> ByteData;
	
};

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class USaveInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class PIPPOP_API ISaveInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
};
