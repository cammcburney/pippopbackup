// Fill out your copyright notice in the Description page of Project Settings.


#include "Mode/PipPopGameInstance.h"
#include "OnlineSubsystemUtils.h"
#include "Menu/MenuMode.h"
#include "Data/Save/PipPopSaveGame.h"
#include "Kismet/GameplayStatics.h"
#include "OnlineSubsystem.h"
#include "Online/OnlineSessionNames.h"
#include "Interfaces/OnlineSessionInterface.h"

UPipPopGameInstance::UPipPopGameInstance()
{
	SaveGameObject = Cast<UPipPopSaveGame>(UGameplayStatics::LoadGameFromSlot(SaveName, 0));
	if (!SaveGameObject)
	{
		CreateSaveSlot();
	}
	CreateSessionCompleteDelegate = FOnCreateSessionCompleteDelegate::CreateUObject(this, &UPipPopGameInstance::HostedSession);
	FindSessionsCompleteDelegate = FOnFindSessionsCompleteDelegate::CreateUObject(this, &UPipPopGameInstance::FindSessionsComplete);
	JoinSessionCompleteDelegate = FOnJoinSessionCompleteDelegate::CreateUObject(this, &UPipPopGameInstance::JoinPlayerSessionComplete);
	EndSessionCompleteDelegate = FOnEndSessionCompleteDelegate::CreateUObject(this, &UPipPopGameInstance::EndSessionCompleted);
	DestroySessionCompleteDelegate = FOnDestroySessionCompleteDelegate::CreateUObject(this, &UPipPopGameInstance::DestroySessionComplete);
}

void UPipPopGameInstance::SaveCharacterCustomisation(TArray<int32> MeshIndexes)
{
	SaveGameObject->CharacterMeshes = MeshIndexes;
}

void UPipPopGameInstance::CreateSaveSlot()
{
	SaveGameObject = Cast<UPipPopSaveGame>(UGameplayStatics::CreateSaveGameObject(UPipPopSaveGame::StaticClass()));
	if (SaveGameObject)
	{
		SaveGameObject->SetPlayerID(FGuid::NewGuid());
		FPlayerSaveData NewPlayerSaveData = FPlayerSaveData();
		NewPlayerSaveData.SetPlayerName("New Player");
		SetPlayerSaveData(NewPlayerSaveData);
	}
}

void UPipPopGameInstance::SaveGame()
{
	if (SaveGameObject)
	{
		UGameplayStatics::SaveGameToSlot(SaveGameObject, SaveName, 0);
	}
	else
	{
		CreateSaveSlot();
	}
}


void UPipPopGameInstance::SetPlayerSaveData(const FPlayerSaveData& PlayerData)
{
	if (SaveGameObject)
	{
		const FGuid PlayerID = SaveGameObject->GetPlayerID();
		TMap<FGuid, FPlayerSaveData> CurrentPlayerSaveData = SaveGameObject->GetPlayerSaveData();
		if (CurrentPlayerSaveData.Contains(PlayerID))
		{
			FPlayerSaveData& ExistingData = CurrentPlayerSaveData[PlayerID];
			ExistingData.SetPlayerName(PlayerData.GetPlayerName());
		}
		else
		{
			CurrentPlayerSaveData.Add(PlayerID, PlayerData);
		}
		SaveGameObject->SetPlayerSaveData(CurrentPlayerSaveData);
		SaveGame();
	}
}

FName UPipPopGameInstance::GetPlayerName()
{
	FName PlayerName = FName("Unknown Player");
	if (SaveGameObject)
	{
		const FGuid PlayerID = SaveGameObject->GetPlayerID();
		TMap<FGuid, FPlayerSaveData> CurrentPlayerSaveData = SaveGameObject->GetPlayerSaveData();
		if (CurrentPlayerSaveData.Contains(PlayerID))
		{
			const FPlayerSaveData& ExistingData = CurrentPlayerSaveData[PlayerID];
			PlayerName = ExistingData.GetPlayerName();
		}
	}
	return PlayerName;
}

void UPipPopGameInstance::SetPlayerName(const FName& NewPlayerName)
{
	if (SaveGameObject && !NewPlayerName.IsNone())
	{
		const FGuid PlayerID = SaveGameObject->GetPlayerID();
		TMap<FGuid, FPlayerSaveData> CurrentPlayerSaveData = SaveGameObject->GetPlayerSaveData();
		if (CurrentPlayerSaveData.Contains(PlayerID))
		{
			FPlayerSaveData& ExistingData = CurrentPlayerSaveData[PlayerID];
			ExistingData.SetPlayerName(NewPlayerName);
			SaveGameObject->SetPlayerSaveData(CurrentPlayerSaveData);
			SaveGame();
		}
	}
}

FPlayerSaveData UPipPopGameInstance::LoadPlayerSaveData()
{
	if (SaveGameObject)
	{
		const FGuid PlayerID = SaveGameObject->GetPlayerID();
		TMap<FGuid, FPlayerSaveData> CurrentPlayerSaveData = SaveGameObject->GetPlayerSaveData();
		if (CurrentPlayerSaveData.Contains(PlayerID))
		{
			return CurrentPlayerSaveData[PlayerID];
		}
	}
	return FPlayerSaveData();
}

void UPipPopGameInstance::HostSession(const FName& SessionName)
{
	if (const IOnlineSessionPtr OnlineSessionInterface =  Online::GetSessionInterface(GetWorld()))
	{
		if (OnlineSessionInterface->GetNamedSession(SessionName))
		{
			OnlineSessionInterface->DestroySession(SessionName);
			// add delegate for session destroyed to unlock host button
			return;
		}
		CreateSessionCompleteDelegateHandle = OnlineSessionInterface->AddOnCreateSessionCompleteDelegate_Handle(CreateSessionCompleteDelegate);
		SessionSettings = MakeShareable(new FOnlineSessionSettings());
		SessionSettings->bIsLANMatch = false;
		SessionSettings->bUsesPresence = true;
		SessionSettings->bIsDedicated = false;
		SessionSettings->bAllowInvites = true;
		SessionSettings->bAllowJoinInProgress = true;
		SessionSettings->bAllowJoinViaPresence = true;
		SessionSettings->bAllowJoinViaPresenceFriendsOnly = false;
		SessionSettings->bShouldAdvertise = true;
		SessionSettings->NumPublicConnections = 16;
		SessionSettings->NumPrivateConnections = 2;
		SessionSettings->bUseLobbiesIfAvailable = true;
		SessionSettings->Set(FName(TEXT("ServerName")), FOnlineSessionSetting(SessionName.ToString(), EOnlineDataAdvertisementType::ViaOnlineService));
		
		const ULocalPlayer* LocalPlayer = GetWorld()->GetFirstLocalPlayerFromController();
		if (OnlineSessionInterface->CreateSession(*LocalPlayer->GetPreferredUniqueNetId(), SessionName, *SessionSettings))
		{
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Hosting Session %s"), *SessionName.ToString()));
		}
	}
}

void UPipPopGameInstance::HostedSession(FName SessionName, bool bHostedSession)
{
	if (const IOnlineSessionPtr OnlineSessionInterface =  Online::GetSessionInterface(GetWorld()))
	{
		OnlineSessionInterface->ClearOnCreateSessionCompleteDelegate_Handle(CreateSessionCompleteDelegateHandle);
	}
	GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Green, FString::Printf(TEXT("HOSTED SESSION - SUCCESSFULLY: %s, NAME: %s"), bHostedSession ? TEXT("true") : TEXT("false"), *SessionName.ToString()));
	if (bHostedSession)
	{
		LoadLevel(FName("LVL_SpaceStation"));
	}
}

void UPipPopGameInstance::FindSessions()
{
	if (const IOnlineSessionPtr OnlineSessionInterface =  Online::GetSessionInterface(GetWorld()))
	{
		FindSessionsCompleteHandle = OnlineSessionInterface->AddOnFindSessionsCompleteDelegate_Handle(FindSessionsCompleteDelegate);
		
		SearchSettings = MakeShareable(new FOnlineSessionSearch());
		SearchSettings->bIsLanQuery = false;
		SearchSettings->MaxSearchResults = 100;
		SearchSettings->TimeoutInSeconds = 120.f;
		SearchSettings->QuerySettings.Set(SEARCH_LOBBIES,true, EOnlineComparisonOp::Equals);
		const ULocalPlayer* LocalPlayer = GetWorld()->GetFirstLocalPlayerFromController();
		if (OnlineSessionInterface->FindSessions(*LocalPlayer->GetPreferredUniqueNetId(), SearchSettings.ToSharedRef()))
		{
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Search started")));
		}
		else
		{
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Failed Searching"));
		}
	}
}

void UPipPopGameInstance::FindSessionsComplete(bool bSearchCompleted)
{
	if (!bSearchCompleted) {return;}
	if (const IOnlineSessionPtr OnlineSessionInterface =  Online::GetSessionInterface(GetWorld()))
	{
		OnlineSessionInterface->ClearOnFindSessionsCompleteDelegate_Handle(FindSessionsCompleteHandle);
		if (AMenuMode* GameMode = Cast<AMenuMode>(GetWorld()->GetAuthGameMode()))
		{
			GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Cyan, FString::Printf(TEXT("SearchCompleted: %i"), SearchSettings->SearchResults.Num()));
			for (int32 i = 0; i < SearchSettings->SearchResults.Num(); i++)
			{
				GameMode->DisplaySessionInfo(SearchSettings->SearchResults[i]);
			}
		}
	}
}

void UPipPopGameInstance::JoinPlayerSession(const FOnlineSessionSearchResult& SearchResult)
{
	if (const IOnlineSessionPtr OnlineSessionInterface =  Online::GetSessionInterface(GetWorld()))
	{
		if (const FOnlineSessionSetting* Setting = SearchResult.Session.SessionSettings.Settings.Find(FName(TEXT("ServerName"))))
		{
			FString ServerNameString;
			Setting->Data.GetValue(ServerNameString);
			const ULocalPlayer* LocalPlayer = GetWorld()->GetFirstLocalPlayerFromController();
			JoinSessionCompleteDelegateHandle = OnlineSessionInterface->AddOnJoinSessionCompleteDelegate_Handle(JoinSessionCompleteDelegate);
			OnlineSessionInterface->JoinSession(*LocalPlayer->GetPreferredUniqueNetId(), FName(ServerNameString), SearchResult);
		}
	}
}

void UPipPopGameInstance::JoinPlayerSessionComplete(FName SessionName, EOnJoinSessionCompleteResult::Type ResultType)
{
	GEngine->AddOnScreenDebugMessage(0, 30.f, FColor::Red, FString::Printf(TEXT("Join Result: %d"), (int32)(ResultType)));
	if (const IOnlineSessionPtr OnlineSessionInterface =  Online::GetSessionInterface(GetWorld()))
	{
		OnlineSessionInterface->ClearOnJoinSessionCompleteDelegate_Handle(JoinSessionCompleteDelegateHandle);
		const bool bHasTravelled = TravelToSession(SessionName);
		if (bHasTravelled)
		{
			UE_LOG(LogTemp, Warning, TEXT("Travel successful: %d"), bHasTravelled)
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Travel failed: %d"), bHasTravelled)
		}
	}
}

void UPipPopGameInstance::EndSession(const FName& SessionName)
{
	if (const IOnlineSessionPtr OnlineSessionInterface =  Online::GetSessionInterface(GetWorld()))
	{
		EndSessionCompleteDelegateHandle = OnlineSessionInterface->AddOnEndSessionCompleteDelegate_Handle(EndSessionCompleteDelegate);
		OnlineSessionInterface->EndSession(SessionName);
	}
}

void UPipPopGameInstance::EndSessionCompleted(const FName SessionName, bool bSessionEnded)
{
	if (const IOnlineSessionPtr OnlineSessionInterface =  Online::GetSessionInterface(GetWorld()))
	{
		OnlineSessionInterface->ClearOnEndSessionCompleteDelegate_Handle(EndSessionCompleteDelegateHandle);
		DestroySession(SessionName);
	}
}

void UPipPopGameInstance::DestroySession(const FName& SessionName)
{
	if (const IOnlineSessionPtr OnlineSessionInterface =  Online::GetSessionInterface(GetWorld()))
	{
		DestroySessionCompleteDelegateHandle = OnlineSessionInterface->AddOnDestroySessionCompleteDelegate_Handle(DestroySessionCompleteDelegate);
		OnlineSessionInterface->DestroySession(SessionName);
	}
}

void UPipPopGameInstance::DestroySessionComplete(FName SessionName, bool bSuccess)
{
	if (const IOnlineSessionPtr OnlineSessionInterface =  Online::GetSessionInterface(GetWorld()))
	{
		OnlineSessionInterface->ClearOnDestroySessionCompleteDelegate_Handle(DestroySessionCompleteDelegateHandle);
	}
}

bool UPipPopGameInstance::TravelToSession(const FName SessionName)
{
	if (const IOnlineSessionPtr OnlineSessionInterface =  Online::GetSessionInterface(GetWorld()))
	{
		FString ConnectionInfo;
		GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::White, FString::Printf(TEXT("Session Name: %s"), *SessionName.ToString()));
		if (OnlineSessionInterface->GetResolvedConnectString(SessionName, ConnectionInfo))
		{
			GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::White, FString::Printf(TEXT("ConnectionInfo Name: %s"), *ConnectionInfo));
			if (APlayerController* PlayerController = GetWorld()->GetFirstPlayerController())
			{
				PlayerController->ClientTravel(ConnectionInfo, TRAVEL_Absolute);
				return true;
			}
		}
	}
	return false;
}

void UPipPopGameInstance::LoadLevel(const FName LevelName)
{
	GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Red, FString::Printf(TEXT("Load Level: %s"), *LevelName.ToString()));
	FString ListenServerMap = LevelName.ToString() + TEXT("?Listen");
	GetWorld()->ServerTravel(ListenServerMap, false, false);
}

