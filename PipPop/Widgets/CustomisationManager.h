// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CustomisationManager.generated.h"

class UPipPopGameInstance;
class UAppearanceSubsystem;
class ACustomisationPawn;
class UAppearanceComponent;
class USwitchMenuItem;
class UCanvasPanel;
class UVerticalBox;
/**
 * 
 */
UCLASS()
class PIPPOP_API UCustomisationManager : public UUserWidget
{
	GENERATED_BODY()

	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UCanvasPanel> CanvasPanel;

	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UVerticalBox> MeshPanel;

	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UVerticalBox> MaterialPanel;
	
	UPROPERTY()
	TObjectPtr<UAppearanceSubsystem> AppearanceSubsystem;
	
	UPROPERTY()
	TObjectPtr<UAppearanceComponent> Appearance;

	UPROPERTY(EditAnywhere)
	TSubclassOf<USwitchMenuItem> SwitchMenuItemClass;
	
public:

	virtual void Init();

	UFUNCTION()
	UAppearanceSubsystem* GetAppearanceSubsystem() {return AppearanceSubsystem;};
};
