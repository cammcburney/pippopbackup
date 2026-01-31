// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "InputActionValue.h"
#include "Camera/CameraComponent.h"
#include "Components/WidgetInteractionComponent.h"
#include "MenuPawn.generated.h"

class UCameraComponent;
class USpringArmComponent;
class UEnhancedInputComponent;
class UInputAction;
class UInputMappingContext;

USTRUCT()
struct FMenuInputActions
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UInputAction> ClickAction;
	
};

UCLASS()
class PIPPOP_API AMenuPawn : public APawn
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly)
	FMenuInputActions InputActions;

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UInputMappingContext> InputMappingContext;

	UPROPERTY(EditAnywhere)
	TObjectPtr<USpringArmComponent> SpringArm;
	
	UPROPERTY(EditAnywhere)
	TObjectPtr<UCameraComponent> CameraComponent;

	UPROPERTY()
	TObjectPtr<UWidgetInteractionComponent> InteractionComponent;
	
public:

	AMenuPawn();

	UFUNCTION()
	void OnLeftClick(const FInputActionValue& Value);

	FRotator GetCameraRotation() const {if (CameraComponent) {return CameraComponent->GetRelativeRotation();} return FRotator::ZeroRotator;}
	
	void SetCameraRotation(const FRotator& CameraRotation) const;

protected:

	virtual void BeginPlay() override;

public:
	

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
