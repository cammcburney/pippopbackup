// Fill out your copyright notice in the Description page of Project Settings.


#include "Mode/AppearanceSubsystem.h"
#include "Mode/PipPopGameInstance.h"
#include "Data/AppearanceStructs.h"

UAppearanceSubsystem::UAppearanceSubsystem()
{
}


void UAppearanceSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
	GameInstance = Cast<UPipPopGameInstance>(GetGameInstance());
	check(GameInstance)
	AppearanceTable = GameInstance->GetAppearanceTable();
	check(AppearanceTable)
}

int32 UAppearanceSubsystem::GetSectionLength(const FName RowName) const
{
	if (const FAppearanceInfo* AppearanceRow = AppearanceTable->FindRow<FAppearanceInfo>(RowName, ""))
	{
		return AppearanceRow->Mesh.Num();
	}
	return 0;
}

FName UAppearanceSubsystem::GetSectionName(const FName RowName) const
{
	if (const FAppearanceInfo* AppearanceRow = AppearanceTable->FindRow<FAppearanceInfo>(RowName, ""))
	{
		return AppearanceRow->SectionName;
	}
	return FName();
}