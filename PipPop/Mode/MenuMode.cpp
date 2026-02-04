// Fill out your copyright notice in the Description page of Project Settings.


#include "MenuMode.h"
#include "OnlineSessionSettings.h"
#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"
#include "UI/Widgets/SessionEntry.h"

void AMenuMode::BeginPlay()
{
	Super::BeginPlay();
	if (APlayerController* Controller = UGameplayStatics::GetPlayerController(this, 0))
	{
		Controller->SetShowMouseCursor(true);
	}
}

void AMenuMode::DisplaySessionInfo(FOnlineSessionSearchResult& FoundSession)
{
	USessionEntry* SessionWidget = CreateWidget<USessionEntry>(GetWorld(), SessionEntryClass);
	const int32 TotalPlayers = FoundSession.Session.SessionSettings.NumPublicConnections + FoundSession.Session.SessionSettings.NumPrivateConnections;
	const int32 OpenPlayers = FoundSession.Session.NumOpenPublicConnections + FoundSession.Session.NumOpenPrivateConnections;
	FText ServerName = FText::FromString("Unknown");
	if (const FOnlineSessionSetting* Setting = FoundSession.Session.SessionSettings.Settings.Find(FName(TEXT("ServerName"))))
	{
		FString ServerNameString;
		Setting->Data.GetValue(ServerNameString);
		ServerName = FText::FromString(ServerNameString);
	}
	const FText CurrentPlayersText = FText::FromString(FString::FromInt(TotalPlayers - OpenPlayers));
	const FText MaxPlayersText = FText::FromString(FString::FromInt(TotalPlayers));
	
	if (SessionWidget)
	{
		SessionWidget->SetSessionData(FoundSession);
		SessionWidget->SetSessionText(ESessionText::CurrentPlayers, CurrentPlayersText);
		SessionWidget->SetSessionText(ESessionText::MaxPlayers, MaxPlayersText);
		SessionWidget->SetSessionText(ESessionText::ServerName, ServerName);
		CreateSessionWidgetDelegate.ExecuteIfBound(SessionWidget);
	}
}