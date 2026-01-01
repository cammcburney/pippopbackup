// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Data/AppearanceStructs.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "AppearanceSubsystem.generated.h"

class UPipPopGameInstance;

/**
 * 
 */
UCLASS()
class PIPPOP_API UAppearanceSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere)
	TObjectPtr<UPipPopGameInstance> GameInstance;

	UPROPERTY(EditAnywhere)
	TObjectPtr<UDataTable> AppearanceTable;

public:
	
	UAppearanceSubsystem();

	int32 GetSectionLength(FName RowName) const;

	FName GetSectionName(FName RowName) const;
	
	template<typename AppearanceItem>
	AppearanceItem* LoadAppearanceAsset(const FName RowName, int32 Index, TArray<TObjectPtr<AppearanceItem>> FAppearanceInfo::*Member);

private:
	
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	
};

template<typename AppearanceItem>
	AppearanceItem* UAppearanceSubsystem::LoadAppearanceAsset(const FName RowName, int32 Index, TArray<TObjectPtr<AppearanceItem>> FAppearanceInfo::*Member)
{
	if (FAppearanceInfo* AppearanceRow = AppearanceTable->FindRow<FAppearanceInfo>(RowName, ""))
	{
		TArray<TObjectPtr<AppearanceItem>>& AppearanceItems = AppearanceRow->*Member;
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
