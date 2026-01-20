// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/Subsystem.h"
#include "LevelSubsystem.generated.h"

/**
 * 
 */
UCLASS()
class PIPPOP_API ULevelSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:

	FName GetLevelName(const FName RowName) const;
	
private:

	UPROPERTY()
	TObjectPtr<UDataTable> LevelTable;
	
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	
};
