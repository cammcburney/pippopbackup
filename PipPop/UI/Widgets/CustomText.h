// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/EditableTextBox.h"
#include "CustomText.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FEnterTextDelegate, FText&, Text);
/**
 * 
 */
UCLASS()
class PIPPOP_API UCustomText : public UUserWidget
{
	GENERATED_BODY()

public:

	virtual void NativePreConstruct() override;
	
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UEditableTextBox> EditableTextBox;

	UPROPERTY()
	FEnterTextDelegate EnterTextDelegate;
	
};
