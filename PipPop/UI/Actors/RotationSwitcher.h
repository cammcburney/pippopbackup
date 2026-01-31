// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/Actors/SwitcherBase.h"
#include "RotationSwitcher.generated.h"

class UGameplayStatics;
class AMenuPawn;
class UTimelineComponent;

/**
 * 
 */
UCLASS()
class PIPPOP_API ARotationSwitcher : public ASwitcherBase
{
	GENERATED_BODY()

public:

	UPROPERTY(EditInstanceOnly)
	bool InvertRotation = false;
	
private:

	UPROPERTY()
	FRotator CameraRotation = FRotator::ZeroRotator;
	
	UPROPERTY()
	TObjectPtr<UTimelineComponent> Timeline;
	
public:
	
	ARotationSwitcher();
	
	virtual void Interact_Implementation(UPrimitiveComponent* InteractedComponent) override;
	
protected:

	virtual void BeginPlay() override;
	
private:
	
	UFUNCTION()
	void ProgressRotation(float Value) const;

	UFUNCTION()
	void RotationCompleted() const;
};
