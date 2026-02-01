// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/Actors/LevelSwitcher.h"
#include "OnlineSessionSwitcher.generated.h"

class UWidgetComponent;
class UMapMenu;
class UMediaPlayer;
class UMediaSource;
class UText3DComponent;
class UCustomText;
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
	TSubclassOf<UCustomText> HiddenTextWidgetClass;

	UPROPERTY()
	TObjectPtr<UCustomText> CustomTextWidget;
	
	UPROPERTY(EditAnywhere)
	TObjectPtr<UText3DComponent> SessionName;
	
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

	UFUNCTION()
	void SessionNameTextUpdated(const FText& NewText);

	UFUNCTION()
	void SessionNameTextCommitted(const FText& Text, ETextCommit::Type CommitMethod);
private:

	void SetLevelReference();
};
