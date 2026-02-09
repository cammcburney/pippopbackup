// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/Actors/SwitcherBase.h"
#include "NameSwitcher.generated.h"

class UWidgetComponent;
class UText3DComponent;
class UCustomText;
/**
 * 
 */
UCLASS()
class PIPPOP_API ANameSwitcher : public ASwitcherBase 
{
	GENERATED_BODY()

public:

	ANameSwitcher();

	UPROPERTY(EditAnywhere)
	int32 MaxStringLength = 20;
	
private:
	
	UPROPERTY(EditInstanceOnly)
	TSubclassOf<UCustomText> HiddenTextWidgetClass;

	UPROPERTY()
	TObjectPtr<UCustomText> CustomTextWidget;
	
public:
	
	virtual void BeginPlay() override;

	virtual void Interact_Implementation(UPrimitiveComponent* InteractedComponent) override;

	UFUNCTION()
	void PlayerNameTextUpdated(const FText& NewText);

	UFUNCTION()
	void PlayerNameTextCommitted(const FText& Text, ETextCommit::Type CommitMethod);

};
