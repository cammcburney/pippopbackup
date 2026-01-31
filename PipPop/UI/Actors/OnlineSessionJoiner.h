// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "UI/Actors/SwitcherBase.h"
#include "UI/Widgets/SessionEntry.h"
#include "OnlineSessionJoiner.generated.h"

class UWidgetComponent;
class USessionsDisplay;

UCLASS()
class PIPPOP_API AOnlineSessionJoiner : public ASwitcherBase
{
	GENERATED_BODY()

	UPROPERTY(EditInstanceOnly)
	TSubclassOf<UUserWidget> SessionsDisplayClass;

	UPROPERTY(EditInstanceOnly)
	TObjectPtr<UWidgetComponent> SessionsDisplayComponent;
	
public:	

	AOnlineSessionJoiner();

	virtual void Interact_Implementation(UPrimitiveComponent* InteractedComponent) override;

	void AddMenuItem(USessionEntry* SessionEntry);
	
protected:

	virtual void BeginPlay() override;

public:	

	virtual void Tick(float DeltaTime) override;

};
