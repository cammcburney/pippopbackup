// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "ShooterController.generated.h"

class UChatBox;
class UEditableTextBox;
class AShooterState;

/**
 * 
 */
UCLASS()
class PIPPOP_API AShooterController : public APlayerController
{
	GENERATED_BODY()
public:

	UPROPERTY(EditAnywhere)
	TSubclassOf<UChatBox> PlayerChatBoxClass;
	
private:

	UPROPERTY(EditAnywhere)
	TObjectPtr<UChatBox> PlayerChatBox;

public:

	void FocusChat();
	
	UFUNCTION(Server, Reliable, WithValidation)
	void ServerSendChatMessage(const FText& PlayerName,  const FText& ChatMessage);
	void ServerSendChatMessage_Implementation(const FText& PlayerName,  const FText& ChatMessage);
	void ServerSendChatMessage_Validation(const FText& PlayerName,  const FText& ChatMessage);

	UFUNCTION(Client, Reliable)
	void UpdateOwningClientChat(APawn* SpeakingPawn, const FText& PlayerName,const FText& ChatMessage);

	
protected:

	virtual void BeginPlay() override;
	
	UFUNCTION(Server, Reliable)
	void ServerSpawnPlayer(APlayerController* PlayerController);
	void ServerSpawnPlayer_Implementation(APlayerController* PlayerController);
};
