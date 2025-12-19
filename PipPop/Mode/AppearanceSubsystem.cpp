// Fill out your copyright notice in the Description page of Project Settings.


#include "Mode/AppearanceSubsystem.h"

#include "Data/AppearanceStructs.h"
#include "Mode/PipPopGameInstance.h"

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

USkeletalMesh* UAppearanceSubsystem::LoadAppearanceItem(const FName RowName, int32 Index)
{
	if (FAppearanceInfo* AppearanceRow = AppearanceTable->FindRow<FAppearanceInfo>(RowName, ""))
	{
		TArray<TObjectPtr<USkeletalMesh>>& AppearanceItems = AppearanceRow->Mesh;
		if (AppearanceItems.Num() == 0) {return nullptr;}
		if (Index < 0)
		{
			Index = AppearanceItems.Num() - 1;
		}
		else if (Index >= AppearanceItems.Num())
		{
			Index = 0;
		}
		return AppearanceItems[Index];
	}
	return nullptr;
}

UMaterialInterface* UAppearanceSubsystem::LoadAppearanceMaterial(const FName RowName, int32 Index)
{
	if (FAppearanceInfo* AppearanceRow = AppearanceTable->FindRow<FAppearanceInfo>(RowName, ""))
	{
		TArray<TObjectPtr<UMaterialInterface>>& AppearanceItems = AppearanceRow->Material;
		if (AppearanceItems.Num() == 0) {return nullptr;}
		if (Index < 0)
		{
			Index = AppearanceItems.Num() - 1;
		}
		else if (Index >= AppearanceItems.Num())
		{
			Index = 0;
		}
		return AppearanceItems[Index];
	}
	return nullptr;
}

int32 UAppearanceSubsystem::GetSectionLength(FName RowName)
{
	if (FAppearanceInfo* AppearanceRow = AppearanceTable->FindRow<FAppearanceInfo>(RowName, ""))
	{
		return AppearanceRow->Mesh.Num();
	}
	return 0;
}