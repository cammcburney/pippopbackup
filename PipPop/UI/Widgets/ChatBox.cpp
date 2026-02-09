// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Widgets/ChatBox.h"

#include "ChatMessage.h"
#include "Character/State/ShooterPlayerState.h"
#include "Components/ScrollBox.h"
#include "Components/EditableTextBox.h"
#include "Controller/ShooterController.h"

void UChatBox::NativeConstruct()
{
	SetIsFocusable(true);
	if (ChatInputBox)
	{
		ChatInputBox->OnTextChanged.AddDynamic(this, &UChatBox::OnTextChanged);
		ChatInputBox->OnTextCommitted.AddDynamic(this, &UChatBox::OnTextCommited);
	}
}

void UChatBox::OnTextChanged(const FText& NewText)
{
}

void UChatBox::OnTextCommited(const FText& Text, ETextCommit::Type CommitMethod)
{
	if (CommitMethod == ETextCommit::Type::OnEnter)
	{
		if (AShooterController* PlayerController = Cast<AShooterController>(GetOwningPlayer()))
		{
			FText PlayerName = FText::FromString("Unknown Player");
			if (const AShooterPlayerState* PlayerState = Cast<AShooterPlayerState>(PlayerController->PlayerState))
			{
				const FString Name = PlayerState->GetPlayerName();
				GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Emerald, FString::Printf(TEXT("CHAT BOX NAME %s"), *Name));
				PlayerName = FText::FromString(Name);
			}
			PlayerController->ServerSendChatMessage(PlayerName, Text);
			PlayerController->SetInputMode(FInputModeGameOnly());
			PlayerController->SetShowMouseCursor(false);
			ChatInputBox->SetText(FText::GetEmpty());
			
		}
	}
}

void UChatBox::UpdateChat(APawn* SpeakingPawn, const FText& PlayerName, const FText& ChatMessage)
{
	if (ChatScrollBox && ChatMessageWidgetClass)
	{
		if (UChatMessage* MessageWidget = CreateWidget<UChatMessage>(this, ChatMessageWidgetClass))
		{
			MessageWidget->SetPlayerName(PlayerName);
			MessageWidget->SetPawn(SpeakingPawn);
			MessageWidget->SetChatMessage(ChatMessage);
			ChatScrollBox->AddChild(MessageWidget);
			ChatScrollBox->ScrollToEnd();
		}
	}
}

void UChatBox::FocusInputBox() const
{
	if (IsValid(ChatInputBox))
	{
		ChatInputBox->SetKeyboardFocus();
	}
}
