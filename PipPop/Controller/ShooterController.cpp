// Fill out your copyright notice in the Description page of Project Settings.


#include "Controller/ShooterController.h"
#include "GameFramework/PlayerState.h"
#include "Mode/Gameplay/ShooterState.h"
#include "Components/EditableTextBox.h"
#include "UI/Widgets/ChatBox.h"
#include "Mode/Gameplay/ShooterMode.h"


void AShooterController::BeginPlay()
{
	Super::BeginPlay();
	bReplicates = true;
	if (IsLocalPlayerController())
	{
		ServerSpawnPlayer(this);
		if (PlayerChatBoxClass)
		{
			if (UChatBox* ChatBox = CreateWidget<UChatBox>(this, PlayerChatBoxClass))
			{
				PlayerChatBox = ChatBox;
				PlayerChatBox->AddToViewport();
			}
		}
	}
	SetInputMode(FInputModeGameOnly());
}

void AShooterController::OnPossess(APawn* NewPawn)
{
	Super::OnPossess(NewPawn);
	FInputModeGameOnly GameInput;
	SetInputMode(GameInput);
	SetShowMouseCursor(false);
}

void AShooterController::ServerSpawnPlayer_Implementation(APlayerController* PlayerController)
{
	if (PlayerController)
	{
		if (AShooterMode* GameMode = Cast<AShooterMode>(GetWorld()->GetAuthGameMode()))
		{
			GameMode->SpawnPlayer(PlayerController);
		}
	}
}

void AShooterController::FocusChat()
{
	SetInputMode(FInputModeUIOnly());
	SetShowMouseCursor(true);
	if (IsValid(PlayerChatBox))
	{
		PlayerChatBox->FocusInputBox();
	}
}

void AShooterController::ServerSendChatMessage_Implementation(const FText& PlayerName, const FText& ChatMessage)
{
	if (AShooterState* GameState = Cast<AShooterState>(GetWorld()->GetGameState()))
	{
		APawn* SpeakingPawn = this->GetPawn();
		TArray<TObjectPtr<APlayerState>> PlayerStates = GameState->PlayerArray;
		for (int i = 0; i < PlayerStates.Num(); i++)
		{
			const APlayerState* PlayersState = PlayerStates[i];
			if (AShooterController* ShooterController = Cast<AShooterController>(PlayersState->GetPlayerController()))
			{
				if (SpeakingPawn)
				{
					ShooterController->UpdateOwningClientChat(SpeakingPawn, PlayerName, ChatMessage);
				}
			}
		}
	}
}

bool AShooterController::ServerSendChatMessage_Validate(const FText& PlayerName, const FText& ChatMessage)
{
	return true;
}

void AShooterController::UpdateOwningClientChat_Implementation(APawn* SpeakingPawn, const FText& PlayerName,const FText& ChatMessage)
{
	if (IsValid(PlayerChatBox))
	{
		PlayerChatBox->UpdateChat(SpeakingPawn, PlayerName, ChatMessage);
	}
}
