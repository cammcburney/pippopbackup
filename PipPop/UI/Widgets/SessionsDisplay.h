// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/VerticalBox.h"
#include "UI/Widgets/SessionEntry.h"
#include "SessionsDisplay.generated.h"

class UVerticalBox;
/**
 * 
 */
UCLASS()
class PIPPOP_API USessionsDisplay : public UUserWidget
{
	GENERATED_BODY()

	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UVerticalBox> SessionList;

public:

	virtual void NativeConstruct() override;

	void AddSession(USessionEntry* NewSession) const;
};
