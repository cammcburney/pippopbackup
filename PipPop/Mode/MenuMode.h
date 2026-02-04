// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "UI/Widgets/SessionEntry.h"
#include "MenuMode.generated.h"

DECLARE_DELEGATE_OneParam(FCreateSessionWidget, USessionEntry*)

class FOnlineSession;
/**
 * 
 */
UCLASS()
class PIPPOP_API AMenuMode : public AGameMode
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	TSubclassOf<USessionEntry> SessionEntryClass;
	
public:
	
	void DisplaySessionInfo(FOnlineSessionSearchResult& FoundSession);

	FCreateSessionWidget CreateSessionWidgetDelegate;
	
protected:

	virtual void BeginPlay() override;
	
};
