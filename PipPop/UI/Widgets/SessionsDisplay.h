// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/ScrollBox.h"
#include "UI/Widgets/SessionEntry.h"
#include "SessionsDisplay.generated.h"

class UScrollBox;
/**
 * 
 */
UCLASS()
class PIPPOP_API USessionsDisplay : public UUserWidget
{
	GENERATED_BODY()
	
public:
	
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UScrollBox> SessionList;

	virtual void NativeConstruct() override;

	void AddSession(USessionEntry* NewSession) const;
};
