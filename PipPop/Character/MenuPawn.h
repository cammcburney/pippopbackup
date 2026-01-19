// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "InputActionValue.h"
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
	
public:

	AMenuPawn();

	UFUNCTION()
	void OnLeftClick(const FInputActionValue& Value);

protected:

	virtual void BeginPlay() override;

public:
	

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
