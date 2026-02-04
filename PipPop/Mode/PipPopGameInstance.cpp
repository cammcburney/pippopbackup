// Fill out your copyright notice in the Description page of Project Settings.


#include "Mode/PipPopGameInstance.h"

#include "MenuMode.h"
#include "Data/Save/PipPopSaveGame.h"
#include "Kismet/GameplayStatics.h"
#include "OnlineSubsystem.h"
#include "Interfaces/OnlineSessionInterface.h"

UPipPopGameInstance::UPipPopGameInstance()
{
	SavedGame = Cast<UPipPopSaveGame>(UGameplayStatics::LoadGameFromSlot(SaveName, 0));
	if (!SavedGame)
	{
		SavedGame = Cast<UPipPopSaveGame>(UGameplayStatics::CreateSaveGameObject(UPipPopSaveGame::StaticClass()));
	}
	FindSessionsCompleteDelegate = FOnFindSessionsCompleteDelegate::CreateUObject(this, &UPipPopGameInstance::FindSessionsComplete);
	JoinSessionCompleteDelegate = FOnJoinSessionCompleteDelegate::CreateUObject(this, &UPipPopGameInstance::JoinPlayerSessionComplete);
	EndSessionCompleteDelegate = FOnEndSessionCompleteDelegate::CreateUObject(this, &UPipPopGameInstance::EndSessionCompleted);
	DestroySessionCompleteDelegate = FOnDestroySessionCompleteDelegate::CreateUObject(this, &UPipPopGameInstance::DestroySessionComplete);
}

void UPipPopGameInstance::SaveCharacterCustomisation(TArray<int32> MeshIndexes)
{
	SavedGame->CharacterMeshes = MeshIndexes;
}

void UPipPopGameInstance::HostSession(const FName& SessionName)
{
	if (const IOnlineSessionPtr OnlineSessionInterface = GetSessionInterface())
	{
		if (OnlineSessionInterface->GetNamedSession(SessionName))
		{
			OnlineSessionInterface->DestroySession(SessionName);
			// add delegate for session destroyed to unlock host button
			return;
		}
		SessionSettings = MakeShareable(new FOnlineSessionSettings());
		SessionSettings->bIsLANMatch = true;
		SessionSettings->bUsesPresence = true;
		SessionSettings->bIsDedicated = false;
		SessionSettings->bAllowInvites = true;
		SessionSettings->bAllowJoinInProgress = true;
		SessionSettings->bAllowJoinViaPresence = true;
		SessionSettings->bAllowJoinViaPresenceFriendsOnly = false;
		SessionSettings->bShouldAdvertise = true;
		SessionSettings->bIsDedicated = false;
		SessionSettings->NumPublicConnections = 16;
		SessionSettings->NumPrivateConnections = 2;
		SessionSettings->Set(FName(TEXT("ServerName")), FOnlineSessionSetting(SessionName.ToString(), EOnlineDataAdvertisementType::ViaOnlineService));
		
		const ULocalPlayer* LocalPlayer = GetWorld()->GetFirstLocalPlayerFromController();
		if (OnlineSessionInterface->CreateSession(*LocalPlayer->GetPreferredUniqueNetId(), SessionName, *SessionSettings))
		{
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Hosting Session %s"), *SessionName.ToString()));
		}
	}
}

void UPipPopGameInstance::FindSessions()
{
	if (const IOnlineSessionPtr OnlineSessionInterface = GetSessionInterface())
	{
		FindSessionsCompleteHandle = OnlineSessionInterface->AddOnFindSessionsCompleteDelegate_Handle(FindSessionsCompleteDelegate);
		
		SearchSettings = MakeShareable(new FOnlineSessionSearch());
		SearchSettings->bIsLanQuery = true;
		SearchSettings->MaxSearchResults = 5;
		SearchSettings->PingBucketSize = 30;
		SearchSettings->TimeoutInSeconds = 30.f;
		
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
	if (const IOnlineSessionPtr OnlineSessionInterface = GetSessionInterface())
	{
		OnlineSessionInterface->ClearOnFindSessionsCompleteDelegate_Handle(FindSessionsCompleteHandle);
		if (AMenuMode* GameMode = Cast<AMenuMode>(GetWorld()->GetAuthGameMode()))
		{
			for (int32 i = 0; i < SearchSettings->SearchResults.Num(); i++)
			{
				GameMode->DisplaySessionInfo(SearchSettings->SearchResults[i]);
			}
		}
	}
}

void UPipPopGameInstance::JoinPlayerSession(const FOnlineSessionSearchResult& SearchResult)
{
	if (const IOnlineSessionPtr OnlineSessionInterface = GetSessionInterface())
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
	if (const IOnlineSessionPtr OnlineSessionInterface = GetSessionInterface())
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
	if (const IOnlineSessionPtr OnlineSessionInterface = GetSessionInterface())
	{
		EndSessionCompleteDelegateHandle = OnlineSessionInterface->AddOnEndSessionCompleteDelegate_Handle(EndSessionCompleteDelegate);
		OnlineSessionInterface->EndSession(SessionName);
	}
}

void UPipPopGameInstance::EndSessionCompleted(const FName SessionName, bool bSessionEnded)
{
	if (const IOnlineSessionPtr OnlineSessionInterface = GetSessionInterface())
	{
		OnlineSessionInterface->ClearOnEndSessionCompleteDelegate_Handle(EndSessionCompleteDelegateHandle);
		DestroySession(SessionName);
	}
}

void UPipPopGameInstance::DestroySession(const FName& SessionName)
{
	if (const IOnlineSessionPtr OnlineSessionInterface = GetSessionInterface())
	{
		DestroySessionCompleteDelegateHandle = OnlineSessionInterface->AddOnDestroySessionCompleteDelegate_Handle(DestroySessionCompleteDelegate);
		OnlineSessionInterface->DestroySession(SessionName);
	}
}

void UPipPopGameInstance::DestroySessionComplete(FName SessionName, bool bSuccess)
{
	if (const IOnlineSessionPtr OnlineSessionInterface = GetSessionInterface())
	{
		OnlineSessionInterface->ClearOnDestroySessionCompleteDelegate_Handle(DestroySessionCompleteDelegateHandle);
	}
}

IOnlineSessionPtr UPipPopGameInstance::GetSessionInterface()
{
	if (const IOnlineSubsystem* OnlineSubsystem = IOnlineSubsystem::Get())
	{
		if (const IOnlineSessionPtr OnlineSessionInterface = OnlineSubsystem->GetSessionInterface())
		{
			return OnlineSessionInterface;
		}
	}
	return nullptr;
}
bool UPipPopGameInstance::TravelToSession(const FName SessionName)
{
	if (const IOnlineSessionPtr OnlineSessionInterface = GetSessionInterface())
	{
		FString ConnectionInfo;
		GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::White, FString::Printf(TEXT("Session Name: %s"), *SessionName.ToString()));
		if (OnlineSessionInterface->GetResolvedConnectString(SessionName, ConnectionInfo))
		{
			GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::White, FString::Printf(TEXT("ConnectionInfo Name: %s"), *ConnectionInfo));
			// Remove port from string, leaving only ip
			FString SplitString;
			FString R;
			ConnectionInfo.Split(":", &SplitString, &R);
			GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::White, FString::Printf(TEXT("Modified ConnectionInfo Name: %s"), *SplitString));
			APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
			PlayerController->ClientTravel(SplitString, TRAVEL_Absolute);
			return true;
		}
	}
	return false;
}

void UPipPopGameInstance::LoadLevel(const FName LevelName)
{
	// UGameplayStatics::OpenLevel(this, LevelName, TRAVEL_Absolute);
	GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Red, FString::Printf(TEXT("Load Level: %s"), *LevelName.ToString()));
	GetWorld()->ServerTravel("LVL_SpaceStation?Listen", false, false);
}

