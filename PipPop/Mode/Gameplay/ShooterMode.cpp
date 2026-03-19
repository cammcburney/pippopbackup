// Fill out your copyright notice in the Description page of Project Settings.


#include "Mode/Gameplay/ShooterMode.h"
#include "ShooterState.h"
#include "Character/ShooterPlayerCharacter.h"
#include "Character/State/ShooterPlayerState.h"
#include "Controller/ShooterController.h"
#include "Kismet/GameplayStatics.h"

AShooterMode::AShooterMode()
{
	DefaultPawnClass = nullptr;
	PlayerControllerClass = AShooterController::StaticClass();
	PlayerStateClass = AShooterPlayerState::StaticClass();
	GameStateClass = AShooterState::StaticClass();
}

void AShooterMode::PostLogin(APlayerController* PlayerController)
{
	Super::PostLogin(PlayerController);
	PlayerControllers.Add(PlayerController);
}

void AShooterMode::Logout(AController* Exiting)
{
	Super::Logout(Exiting);
	if (APlayerController* Controller = Cast<APlayerController>(Exiting))
	{
		PlayerControllers.Remove(Controller);
	}
}

void AShooterMode::SwapPlayerControllers(APlayerController* OldPC, APlayerController* NewPC)
{
	Super::SwapPlayerControllers(OldPC, NewPC);
	PlayerControllers.Remove(OldPC);
	PlayerControllers.Add(NewPC);
}

FTransform AShooterMode::FindPlayerStart()
{
	TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsOfClass(this, APlayerStart::StaticClass(),FoundActors);

	PlayerStarts.Reset();
	
	for (AActor* Actor : FoundActors)
	{
		if (APlayerStart* Start = Cast<APlayerStart>(Actor))
		{
			PlayerStarts.AddUnique(Start);	
		}
	}
	if (PlayerStarts.Num() > 0)
	{
		int32 NewIndex = FMath::RandRange(0, PlayerStarts.Num() - 1);
		if (FoundActors.IsValidIndex(NewIndex))
		{
			const FTransform ActorTransform = PlayerStarts[NewIndex]->GetActorTransform();
			return ActorTransform;
		}
	}
	return FTransform();
}

void AShooterMode::SpawnPlayer(APlayerController* PlayerController)
{
	if (PlayerController)
	{
		if (APawn* Pawn = PlayerController->GetPawn())
		{
			Pawn->Destroy();
		}
		if (UWorld* World = GetWorld())
		{
			const FTransform SpawnTransform = FindPlayerStart();
			if (APawn* Pawn = Cast<APawn>(World->SpawnActor(DefaultCharacterClass, &SpawnTransform, FActorSpawnParameters())))
			{
				PlayerController->Possess(Pawn);
			}
		}
	}
}