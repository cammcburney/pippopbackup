// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ChatBox.generated.h"

class UScrollBox;
class UEditableTextBox;
class UChatMessage;
/**
 * 
 */
UCLASS()
class PIPPOP_API UChatBox : public UUserWidget
{
	GENERATED_BODY()
	
public:

	UPROPERTY(EditAnywhere)
	TSubclassOf<UChatMessage> ChatMessageWidgetClass;
	
private:
	
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UScrollBox> ChatScrollBox;

	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UEditableTextBox> ChatInputBox;

public:

	virtual void NativeConstruct();

	void FocusInputBox() const;
	
	UFUNCTION()
	void OnTextChanged(const FText& NewText);

	UFUNCTION()
	void OnTextCommited(const FText& Text, ETextCommit::Type CommitMethod);

	void UpdateChat(APawn* SpeakingPawn, const FText& PlayerName, const FText& ChatMessage);
};
