// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/State/ShooterPlayerState.h"

#include "Mode/PipPopGameInstance.h"

void AShooterPlayerState::BeginPlay()
{
	Super::BeginPlay();
	if (const UWorld* World = GetWorld())
	{
		if (UPipPopGameInstance* GameInstance = Cast<UPipPopGameInstance>(World->GetGameInstance()))
		{
			const FString PlayerName = GameInstance->GetPlayerName().ToString();
			GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Emerald, FString::Printf(TEXT("STATE NAME = %s"), *PlayerName));
			if (!PlayerName.IsEmpty())
			{
				SetPlayerName(PlayerName);
			}
		}
	}
}
