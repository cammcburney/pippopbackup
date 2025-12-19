// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "PipPopGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class PIPPOP_API UPipPopGameInstance : public UGameInstance
{
	GENERATED_BODY()
	
public:
	
	TObjectPtr<UDataTable> GetAppearanceTable() const {return AppearanceTable;} 
	
private:
	
	UPROPERTY(EditDefaultsOnly, meta=(RequiredAssetDataTags = "RowStructure=/Script/PipPop.AppearanceInfo"))
	TObjectPtr<UDataTable> AppearanceTable;
};
