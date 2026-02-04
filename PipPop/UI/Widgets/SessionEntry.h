// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/TextBlock.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "SessionEntry.generated.h"

class UButton;
class UPipPopGameInstance;

UENUM(BlueprintType)
enum class ESessionText : uint8
{
	ServerName,
	MapName,
	ModeName,
	CurrentPlayers,
	MaxPlayers,
	Ping,
};
/**
 * 
 */
UCLASS()
class PIPPOP_API USessionEntry : public UUserWidget
{
	GENERATED_BODY()

	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UTextBlock> ServerName;

	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UTextBlock> MapName;

	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UTextBlock> ModeName;

	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UTextBlock> CurrentPlayers;

	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UTextBlock> MaxPlayers;

	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UTextBlock> Ping;

	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UButton> SelectButton;
	
	FOnlineSessionSearchResult* SessionSearchResult;
	
public:

	virtual void NativePreConstruct() override;

	void SetSessionData(FOnlineSessionSearchResult& FoundSession) {SessionSearchResult = &FoundSession;}
	
	UFUNCTION()
	void SelectSession();
	
	UFUNCTION()
	void SetSessionText(const ESessionText TextType, const FText& Text);

	
};
