// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "CreationGameMode.generated.h"

class UCustomisationManager;
/**
 * 
 */
UCLASS(Blueprintable)
class PIPPOP_API ACreationGameMode : public AGameMode
{
	GENERATED_BODY()

public:

	virtual void BeginPlay() override;
	
};
