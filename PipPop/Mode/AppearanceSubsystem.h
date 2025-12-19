// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
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

	USkeletalMesh* LoadAppearanceItem(const FName RowName, int32 Index);

	UMaterialInterface* LoadAppearanceMaterial(const FName RowName, int32 Index);

	int32 GetSectionLength(FName RowName);
private:
	
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	
};
