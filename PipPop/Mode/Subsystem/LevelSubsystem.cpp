// Fill out your copyright notice in the Description page of Project Settings.


#include "Mode/Subsystem/LevelSubsystem.h"
#include "Mode/PipPopGameInstance.h"
#include "Data/LevelStructs.h"

void ULevelSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
	const UPipPopGameInstance* GameInstance = Cast<UPipPopGameInstance>(GetGameInstance());
	check(GameInstance)
	LevelTable = GameInstance->GetLevelTable();
	check(LevelTable)
}

FName ULevelSubsystem::GetLevelName(const FName RowName) const
{
	if (const FLevelNames* LevelNames = LevelTable->FindRow<FLevelNames>(RowName, ""))
	{
		return LevelNames->LevelName;
	}
	return FName();
}
