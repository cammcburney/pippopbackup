// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/TextBlock.h"
#include "ChatMessage.generated.h"

class UGameplayStatics;
/**
 * 
 */
UCLASS()
class PIPPOP_API UChatMessage : public UUserWidget
{
	GENERATED_BODY()
	
public:

	UPROPERTY(EditAnywhere)
	TObjectPtr<USoundBase> TextSound;

	UPROPERTY(EditAnywhere)
	float ChatDelay = .1f;
	
private:
	
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UTextBlock> PlayerName;

	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UTextBlock> ChatMessage;

	FString NewChatMessage;
	
	FTimerHandle ChatTimerHandle;
	
	int32 CharacterIndex = 0;

	TObjectPtr<APawn> SpeakingPawn;
	
public:

	void SetPawn (APawn* NewPawn) {SpeakingPawn = NewPawn;}
	
	void SetPlayerName(const FText& NewName) const {PlayerName->SetText(NewName);}

	void SetChatMessage(const FText& NewMessage);

private:
	
	void AddNextCharacter();
};
