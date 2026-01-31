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

void AMenuMode::DisplaySessionInfo(const FOnlineSessionSearchResult& FoundSession)
{
	USessionEntry* SessionWidget = CreateWidget<USessionEntry>(GetWorld(), SessionEntryClass);
	const int32 TotalPlayers = FoundSession.Session.SessionSettings.NumPublicConnections + FoundSession.Session.SessionSettings.NumPrivateConnections;
	const int32 OpenPlayers = FoundSession.Session.NumOpenPublicConnections + FoundSession.Session.NumOpenPrivateConnections;
	FText ServerName = FText::FromString("Server");
	if (const FOnlineSessionSetting* Setting = FoundSession.Session.SessionSettings.Settings.Find("ServerName"))
	{
		FString ServerNameString;
		Setting->Data.GetValue(ServerNameString);
		ServerName = FText::FromString(ServerNameString);
		GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Red, FString::Printf(TEXT("Server Name: %s"), *ServerNameString));
	}
	const FText CurrentPlayersText = FText::FromString(FString::FromInt(TotalPlayers - OpenPlayers));
	const FText MaxPlayersText = FText::FromString(FString::FromInt(TotalPlayers));
	
	if (SessionWidget)
	{
		SessionWidget->SetSessionText(ESessionText::CurrentPlayers, CurrentPlayersText);
		SessionWidget->SetSessionText(ESessionText::MaxPlayers, MaxPlayersText);
		SessionWidget->SetSessionText(ESessionText::ServerName, ServerName);
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("AttemptingDisplay")));
		CreateSessionWidgetDelegate.ExecuteIfBound(SessionWidget);
	}
}