// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/Actors/LevelSwitcher.h"
#include "OnlineSessionSwitcher.generated.h"

class UWidgetComponent;
class UMapMenu;
class UMediaPlayer;
class UMediaSource;
class UEditableTextBox;
/**
 * 
 */
UCLASS()
class PIPPOP_API AOnlineSessionSwitcher : public ALevelSwitcher
{
	GENERATED_BODY()

public:

	AOnlineSessionSwitcher();

	UPROPERTY(EditInstanceOnly)
	TObjectPtr<UMediaSource> MapVideo;

	UPROPERTY(EditInstanceOnly)
	TObjectPtr<UEditableTextBox> SessionName;

private:
	
	UPROPERTY(EditInstanceOnly)
	TSubclassOf<UUserWidget> WidgetClass;
	
	UPROPERTY(EditInstanceOnly)
	TObjectPtr<UWidgetComponent> MapMenuComponent;

	UPROPERTY(EditInstanceOnly)
	TObjectPtr<UMediaPlayer> MediaPlayer;

public:

	virtual void BeginPlay() override;

	virtual void Interact_Implementation(UPrimitiveComponent* InteractedComponent) override;
	
private:

	void SetLevelReference();
};
