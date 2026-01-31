// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Data/LevelStructs.h"
#include "MapMenu.generated.h"


class UButton;

DECLARE_DELEGATE(FSetLevelReference);
/**
 * 
 */
UCLASS(Blueprintable)
class PIPPOP_API UMapMenu : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UButton> LeftButton;

	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UButton> RightButton;

	UPROPERTY(EditInstanceOnly)
	int32 GridSize = 6;
	
	UPROPERTY()
	TArray<TObjectPtr<UButton>> Buttons;
	
	UPROPERTY()
	ELevelStructs LevelToLoad = ELevelStructs::Main_Menu;

	FSetLevelReference SetLevelDelegate;
	
private:

	UPROPERTY(meta=(ClampMin=1))
	int32 MapIndexMultiplier = 1;

public:

	virtual void NativeConstruct() override;

	void NextItem(const int32 Iteration);
	
	UFUNCTION()
	void OnLeftClick() {NextItem(-1);};

	UFUNCTION()
	void OnRightClick() {NextItem(1);}
	
	ELevelStructs GetLevel() const { return LevelToLoad; }
	
private:
	
	UFUNCTION()
	void SetLevel();
	
};
