// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Widgets/SessionEntry.h"
#include "Mode/PipPopGameInstance.h"
#include "Components/Button.h"

void USessionEntry::NativePreConstruct()
{
	if (SelectButton)
	{
		SelectButton->OnClicked.AddDynamic(this, &USessionEntry::SelectSession);
	}
}

void USessionEntry::SelectSession()
{
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("USessionEntry::SelectSession"));
	if (SelectButton && SessionSearchResult)
	{
		if (UPipPopGameInstance* GameInstance = Cast<UPipPopGameInstance>(GetWorld()->GetGameInstance()))
		{
			GameInstance->JoinPlayerSession(*SessionSearchResult);
		}
	}
}

void USessionEntry::SetSessionText(const ESessionText TextType, const FText& Text)
{
	switch (TextType)
	{
		case ESessionText::ServerName:
			if (ServerName) {ServerName->SetText(Text);}
			break;
		case ESessionText::MapName:
			if (MapName) {MapName->SetText(Text);}
			break;
		case ESessionText::CurrentPlayers:
			if (CurrentPlayers) {CurrentPlayers->SetText(Text);}
			break;
		case ESessionText::MaxPlayers:
			if (MaxPlayers) {MaxPlayers->SetText(Text);}
			break;
		case ESessionText::ModeName:
			if (ModeName) {ModeName->SetText(Text);}
			break;
		case ESessionText::Ping:
			if (Ping) {Ping->SetText(Text);}
			break;
	}
}

