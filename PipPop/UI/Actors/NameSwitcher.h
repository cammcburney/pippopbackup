// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/Actors/SwitcherBase.h"
#include "NameSwitcher.generated.h"

class UWidgetComponent;
class UText3DComponent;
class UCustomText;
class UTextBlock;

/**
 * 
 */
UCLASS()
class PIPPOP_API ANameSwitcher : public ASwitcherBase 
{
	GENERATED_BODY()

public:

	ANameSwitcher();

	FTimerHandle CaretTimerHandle;

	float ToggleTime = .5f;
	
	UPROPERTY(EditAnywhere)
	int32 MaxStringLength = 20;

	UPROPERTY(EditAnywhere)
	float TextSpacing = 10.f;

	FString LastTextString;
private:

	bool bCaretVisible = false;
	
	UPROPERTY(EditInstanceOnly)
	TSubclassOf<UCustomText> HiddenTextWidgetClass;

	UPROPERTY()
	TObjectPtr<UCustomText> CustomTextWidget;

	UPROPERTY(EditInstanceOnly)
	TObjectPtr<UText3DComponent> Caret;
	
public:

	void ShowCaret();

	void ToggleCaret();
	
	void HideCaret();
	
	virtual void BeginPlay() override;

	virtual void Interact_Implementation(UPrimitiveComponent* InteractedComponent) override;

	UFUNCTION()
	void PlayerNameTextUpdated(const FText& NewText);

	UFUNCTION()
	void PlayerNameTextCommitted(const FText& Text, ETextCommit::Type CommitMethod);
	
};
