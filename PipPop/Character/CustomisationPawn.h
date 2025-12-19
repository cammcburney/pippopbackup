// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "CustomisationPawn.generated.h"

class UAppearanceComponent;
class UCameraComponent;
class USpringArmComponent;

UCLASS()
class PIPPOP_API ACustomisationPawn : public APawn
{
	GENERATED_BODY()

	UPROPERTY()
	TObjectPtr<UAppearanceComponent> AppearanceComponent;

	UPROPERTY(EditAnywhere)
	TObjectPtr<USpringArmComponent> SpringArm;
	
	UPROPERTY(EditAnywhere)
	TObjectPtr<UCameraComponent> CameraComponent;
	
public:
	// Sets default values for this pawn's properties
	ACustomisationPawn();

	UAppearanceComponent* GetAppearance() const {return AppearanceComponent;}

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
