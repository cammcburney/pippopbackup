// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/TextBlock.h"
#include "ChatMessage.generated.h"
/**
 * 
 */
UCLASS()
class PIPPOP_API UChatMessage : public UUserWidget
{
	GENERATED_BODY()

	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UTextBlock> PlayerName;

	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UTextBlock> ChatMessage;

public:

	void SetPlayerName(const FText& NewName) const {PlayerName->SetText(NewName);}

	void SetChatMessage(const FText& NewMessage) const {ChatMessage->SetText(NewMessage);}
};
