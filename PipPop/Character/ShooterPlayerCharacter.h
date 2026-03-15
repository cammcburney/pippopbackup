// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InputActionValue.h"
#include "ShooterPlayerCharacter.generated.h"

class UInputMappingContext;
class UInputAction;
class UInputComponent;
class UCameraComponent;
class UCombatComponent;
class ABaseWeapon;
class UPipPopMovementComponent;

UCLASS(Blueprintable)
class PIPPOP_API AShooterPlayerCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AShooterPlayerCharacter(const FObjectInitializer& ObjectInitializer);

	UPROPERTY(EditAnywhere)
	TSubclassOf<ABaseWeapon> PrimaryWeaponClass;

	UPROPERTY(VisibleAnywhere, Category = Camera)
	TObjectPtr<UCameraComponent> FirstPersonCameraComponent;
	
protected:
	
	UPROPERTY(EditAnywhere, Category = Input)
	TObjectPtr<UInputMappingContext> MappingContext;

	UPROPERTY(EditAnywhere, Category = Input)
	TObjectPtr<UInputAction> MoveAction;

	UPROPERTY(EditAnywhere, Category = Input)
	TObjectPtr<UInputAction> JumpAction;

	UPROPERTY(EditAnywhere, Category = Input)
	TObjectPtr<UInputAction> LookAction;

	UPROPERTY(EditAnywhere, Category = Input)
	TObjectPtr<UInputAction> ChatAction;

	UPROPERTY(EditAnywhere, Category = Input)
	TObjectPtr<UInputAction> AimAction;
	
	UPROPERTY(EditAnywhere, Category = Input)
	TObjectPtr<UInputAction> FireAction;

	UPROPERTY(EditAnywhere, Category = Input)
	TObjectPtr<UInputAction> SprintAction;

	UPROPERTY(EditAnywhere, Category = Input)
	TObjectPtr<UInputAction> SlideAction;
	
	UPROPERTY(EditAnywhere, Replicated, Category = Input, BlueprintReadOnly)
	TObjectPtr<UCombatComponent> CombatComponent;
	
private:

	UPROPERTY()
	TObjectPtr<ABaseWeapon> PrimaryWeapon;

	UPROPERTY(Replicated)
	TObjectPtr<ABaseWeapon> EquippedWeapon;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual void PossessedBy(AController* NewController) override;
	
	virtual void OnRep_Controller() override;
	
	UFUNCTION(BlueprintCallable)
	UCombatComponent* GetCombatComponent() {return CombatComponent;}
	
	void SetupInputMapping();
	
	UFUNCTION()
	void PlayerMove(const FInputActionValue& Value);

	UFUNCTION()
	void PlayerLook(const FInputActionValue& Value);
	
	UFUNCTION()
	void PlayerJump(const FInputActionValue& Value);

	UFUNCTION()
	void PlayerStopJump(const FInputActionValue& Value);

	UFUNCTION()
	void PlayerChat();

	UFUNCTION()
	void PlayerAim();

	UFUNCTION()
	void PlayerStopAiming();
	
	UFUNCTION()
	void PlayerFire(const FInputActionValue& Value);

	UFUNCTION()
	void PlayerSprint(const FInputActionValue& Value);

	UFUNCTION()
	void PlayerStopSprinting();

	UFUNCTION()
	void PlayerSlide();

	UFUNCTION()
	void PlayerStopSliding();
	
	UFUNCTION(Server, Reliable, WithValidation)
	virtual void PlayerDeath();
	virtual void PlayerDeath_Implementation();
	virtual bool PlayerDeath_Validate();
	
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;
protected:
	
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
};
