// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "PipPopMovementComponent.generated.h"

/**
 * 
 */
UCLASS()
class PIPPOP_API UPipPopMovementComponent : public UCharacterMovementComponent
{
	GENERATED_BODY()

public:

	UPipPopMovementComponent();
	
	bool bRequestingSprint : 1;

	bool bCanWallJump : 1;

	FTimerHandle TraceHandle;
	
	void SetCanWallJump(bool bWallJumpState);

	void TraceForWalls();

	UFUNCTION(Server, Reliable, WithValidation)
	virtual void WallJump();
	virtual void WallJump_Implementation();
	virtual bool WallJump_Validate();
	
	void SetSprinting(bool Sprint);
	
	virtual class FNetworkPredictionData_Client* GetPredictionData_Client() const override;
	
	virtual void UpdateFromCompressedFlags(uint8 Flags) override;

	virtual void OnMovementModeChanged(EMovementMode PreviousMovementMode, uint8 PreviousCustomMode) override;

	virtual void OnMovementUpdated(float DeltaSeconds, const FVector& OldLocation, const FVector& OldVelocity) override;
	
	UPROPERTY(EditDefaultsOnly)
	float MaxSprintSpeed = 1500.f;

	virtual float GetMaxSpeed() const override;
	
	UPROPERTY(EditDefaultsOnly)
	int8 MaxWallJumps = 1;
	
	UPROPERTY(EditDefaultsOnly)
	float WallJumpLagSpeed = 1000.f;

	bool HasMaxedWallJumps() const {return CurrentWallJumps >= MaxWallJumps;}

	void ResetMaxedWallJumps();

	virtual FVector NewFallVelocity(const FVector& InitialVelocity, const FVector& Gravity, float DeltaTime) const override;

	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UPROPERTY(EditDefaultsOnly, meta=(ClampMin=10, ClampMax=50))
	float WallJumpForce = 30.f;
	
	UPROPERTY(EditDefaultsOnly)
	float MaxTerminalVelocity = 50000.f;

	UPROPERTY(EditDefaultsOnly, meta=(ClampMin=0, ClampMax=100))
	float GravityScaleMultiplier = 3.3f;

	bool bIsSliding: 1;
	
	UFUNCTION(Server, Reliable, WithValidation)
	virtual void Slide();
	virtual void Slide_Implementation();
	virtual bool Slide_Validate();

	UFUNCTION(Server, Reliable, WithValidation)
	virtual void StopSliding();
	virtual void StopSliding_Implementation();
	virtual bool StopSliding_Validate();
	
private:
	
	FTimerHandle WallJumpCooldown;

	int8 CurrentWallJumps = 0;

	float FallTime = 0.f;
};

class FPipPopSavedMove_Character : public FSavedMove_Character
{
public:
	
	typedef FSavedMove_Character Super;
	
	virtual void Clear() override;

	virtual uint8 GetCompressedFlags() const override;
	
	virtual void SetMoveFor(ACharacter* C, float InDeltaTime, FVector const& NewAccel, class FNetworkPredictionData_Client_Character& ClientData) override;
	
	virtual void PrepMoveFor(ACharacter* C) override;

	virtual bool CanCombineWith(const FSavedMovePtr& NewMove, ACharacter* InCharacter, float MaxDelta) const override;

	uint8 bRequestingSprintSaved : 1;
	
};

class FPipPopNetworkPredictionData_Client_Character : public FNetworkPredictionData_Client_Character
{
public:
	typedef FNetworkPredictionData_Client_Character Super;
	FPipPopNetworkPredictionData_Client_Character(UCharacterMovementComponent& CharacterMovementComponent) : Super(CharacterMovementComponent){}
	virtual FSavedMovePtr AllocateNewMove() override;
};