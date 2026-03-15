// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "GameFramework/PlayerStart.h"
#include "ShooterMode.generated.h"

class AShooterState;
class AShooterPlayerCharacter;
class AShooterPlayerState;
class AShooterController;

/**
 * 
 */
UCLASS()
class PIPPOP_API AShooterMode : public AGameMode
{
	GENERATED_BODY()

public:

	AShooterMode();

	UPROPERTY(EditAnywhere)
	TSubclassOf<AShooterPlayerCharacter> DefaultCharacterClass;
	
private:
	
	UPROPERTY()
	TArray<TObjectPtr<APlayerController>> PlayerControllers;

	UPROPERTY()
	TArray<TObjectPtr<APlayerStart>> PlayerStarts;
	
public:

	FTransform FindPlayerStart();

	void SpawnPlayer(APlayerController* PlayerController);
	
protected:

	virtual void PostLogin(APlayerController* PlayerController) override;

	virtual void Logout(AController* Exiting) override;
	
	virtual void SwapPlayerControllers(APlayerController* OldPC, APlayerController* NewPC) override;
};
