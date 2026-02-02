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
}

void UPipPopGameInstance::SaveCharacterCustomisation(TArray<int32> MeshIndexes)
{
	SavedGame->CharacterMeshes = MeshIndexes;
}

void UPipPopGameInstance::HostSession(const FName& SessionName)
{
	if (const IOnlineSubsystem* OnlineSubsystem = IOnlineSubsystem::Get())
	{
		if (const IOnlineSessionPtr OnlineSessionInterface = OnlineSubsystem->GetSessionInterface())
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
}

void UPipPopGameInstance::FindSessions()
{
	if (const IOnlineSubsystem* OnlineSubsystem = IOnlineSubsystem::Get())
	{
		if (const IOnlineSessionPtr OnlineSessionInterface = OnlineSubsystem->GetSessionInterface())
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
}

void UPipPopGameInstance::FindSessionsComplete(bool bSearchCompleted)
{
	if (!bSearchCompleted) {return;}
	if (const IOnlineSubsystem* OnlineSubsystem = IOnlineSubsystem::Get())
	{
		if (const IOnlineSessionPtr OnlineSessionInterface = OnlineSubsystem->GetSessionInterface())
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
}

void UPipPopGameInstance::LoadLevel(const FName LevelName)
{
	UGameplayStatics::OpenLevel(this, LevelName, TRAVEL_Absolute);
}

