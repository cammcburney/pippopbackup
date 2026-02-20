// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Components/PipPopMovementComponent.h"

#include "Character/ShooterPlayerCharacter.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/PhysicsVolume.h"
#define ECC_WALLJUMP ECC_GameTraceChannel2

UPipPopMovementComponent::UPipPopMovementComponent()
{
	// Check if this breaks things
	bRequestingSprint = false;
	bCanWallJump = false;
}

void UPipPopMovementComponent::SetCanWallJump(bool bWallJumpState)
{
	bCanWallJump = bWallJumpState;
}

void UPipPopMovementComponent::TraceForWalls()
{
	if (const AActor* TraceFromActor = Cast<AActor>(GetOwner()))
	{
		const FVector Start = TraceFromActor->GetActorLocation();
		const FVector End = TraceFromActor->GetActorForwardVector() * 200.f + Start;
		FCollisionQueryParams TraceParams = FCollisionQueryParams();
		TraceParams.bTraceComplex = true;
		TraceParams.bReturnPhysicalMaterial = false;
		FHitResult TraceHit(ForceInit);
		GetWorld()->LineTraceSingleByChannel(
			TraceHit,
			Start,
			End,
			ECC_WALLJUMP,
			TraceParams
		);
		DrawDebugLine(
		GetWorld(),
		Start,
		End,
		FColor(255, 0, 0),
		false, -1, 0,
		12.333
		);
		if (TraceHit.bBlockingHit)
		{
			SetCanWallJump(true);
		}
		else
		{
			SetCanWallJump(false);
		}
	}
}

void UPipPopMovementComponent::WallJump_Implementation()
{
	if (HasMaxedWallJumps())
	{
		if (GetWorld()->GetTimerManager().IsTimerActive(WallJumpCooldown))
		{
			return;
		}
		GetWorld()->GetTimerManager().SetTimer(WallJumpCooldown, this, &UPipPopMovementComponent::ResetMaxedWallJumps, 1, false);
		return;
	}
	if (bCanWallJump)
	{
		if (AShooterPlayerCharacter* PlayerCharacter = Cast<AShooterPlayerCharacter>(GetOwner()))
		{
			PlayerCharacter->LaunchCharacter(FVector(PlayerCharacter->GetActorForwardVector().X * (WallJumpForce * 100), 0.f, JumpZVelocity), true, true);
			CurrentWallJumps += 1;
		}
	}
}

bool UPipPopMovementComponent::WallJump_Validate()
{
	return true;
}

void UPipPopMovementComponent::SetSprinting(bool Sprint)
{
	bRequestingSprint = Sprint;
}

void UPipPopMovementComponent::OnMovementModeChanged(EMovementMode PreviousMovementMode, uint8 PreviousCustomMode)
{
	Super::OnMovementModeChanged(PreviousMovementMode, PreviousCustomMode);
	GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Red, FString::Printf(TEXT("Movement Mode: %s"), *GetMovementName()));
	if (PreviousMovementMode == MOVE_Falling)
	{
		GetWorld()->GetTimerManager().ClearTimer(TraceHandle);
		SetCanWallJump(false);
		if (MovementMode != MOVE_Falling)
		{
			FallTime = 0.f;
		}
	}
	if (MovementMode == MOVE_Falling)
	{
		GetWorld()->GetTimerManager().SetTimer(TraceHandle, this, &UPipPopMovementComponent::TraceForWalls, 0.1f, true);
	}
	
}

void UPipPopMovementComponent::OnMovementUpdated(float DeltaSeconds, const FVector& OldLocation, const FVector& OldVelocity)
{
	Super::OnMovementUpdated(DeltaSeconds, OldLocation, OldVelocity);
	if (bCanWallJump)
	{
		if (MovementMode != MOVE_Falling)
		{
			SetCanWallJump(false);
			
		}
		else
		{
			Velocity = FMath::VInterpConstantTo(Velocity, FVector(0.f, 0.f, 0.f), GetWorld()->GetDeltaSeconds(), WallJumpLagSpeed);
		}	
	}
}


void UPipPopMovementComponent::UpdateFromCompressedFlags(uint8 Flags)
{
	Super::UpdateFromCompressedFlags(Flags);
	bRequestingSprint = (Flags&FSavedMove_Character::FLAG_Custom_0) != 0;
}

float UPipPopMovementComponent::GetMaxSpeed() const
{
	float Speed =  Super::GetMaxSpeed();

	if (bRequestingSprint)
	{
		Speed = MaxSprintSpeed;
	}
	
	return Speed;
}

void UPipPopMovementComponent::ResetMaxedWallJumps()
{
	GetWorld()->GetTimerManager().ClearTimer(WallJumpCooldown);
	CurrentWallJumps = 0;
}

FVector UPipPopMovementComponent::NewFallVelocity(const FVector& InitialVelocity, const FVector& Gravity,
	float DeltaTime) const
{
	GetPhysicsVolume()->TerminalVelocity = MaxTerminalVelocity;
	const FVector ScaledGravity = Gravity * (FallTime * GravityScaleMultiplier);
	FVector FallVelocity = Super::NewFallVelocity(InitialVelocity, ScaledGravity, DeltaTime);
	GEngine->AddOnScreenDebugMessage(-1, 0.f, FColor::Yellow, FString::Printf(TEXT("FALL TIME: %.2f | Velocity %.2f, | Terminal %.2f"), FallTime, FallVelocity.Z, -MaxTerminalVelocity));
	FallVelocity.Z = FMath::Max(FallVelocity.Z, -MaxTerminalVelocity);
	return FallVelocity;
	
}

void UPipPopMovementComponent::TickComponent(float DeltaTime, enum ELevelTick TickType,
	FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	if (MovementMode == MOVE_Falling)
	{
		FallTime += DeltaTime;
	}
}

void UPipPopMovementComponent::Slide_Implementation()
{
	if (!bIsSliding)
	{
		bIsSliding = true;
		if (AShooterPlayerCharacter* PlayerCharacter = Cast<AShooterPlayerCharacter>(GetOwner()))
		{
			UCapsuleComponent* Capsule = PlayerCharacter->GetCapsuleComponent();
			// GroundFriction = GroundFriction / 2;
			if (!Capsule) {bIsSliding = false; return;}
			// float HalfHeight = Capsule->GetScaledCapsuleHalfHeight();
			// const float CapsuleHeight = FMath::Lerp(HalfHeight, HalfHeight / 2,  1.f);
			// Capsule->SetCapsuleHalfHeight(HalfHeight / 2);
			FVector SlideVelocity = (PlayerCharacter->GetActorForwardVector() * PlayerCharacter->GetVelocity().Size2D()) * 1.2f;
			SlideVelocity.Z = -200.f;
			PlayerCharacter->LaunchCharacter(SlideVelocity, true, true);
		}
	}
}

bool UPipPopMovementComponent::Slide_Validate()
{
	return true;
}

void UPipPopMovementComponent::StopSliding_Implementation()
{
	bIsSliding = false;
	// if (AShooterPlayerCharacter* PlayerCharacter = Cast<AShooterPlayerCharacter>(GetOwner()))
	// {
	// 	UCapsuleComponent* Capsule = PlayerCharacter->GetCapsuleComponent();
	// 	if (!Capsule) {bIsSliding = false; return;}
	// 	float HalfHeight = Capsule->GetScaledCapsuleHalfHeight();
	// 	Capsule->SetCapsuleHalfHeight(HalfHeight * 2);
	// }
}

bool UPipPopMovementComponent::StopSliding_Validate()
{
	return true;
}

class FNetworkPredictionData_Client* UPipPopMovementComponent::GetPredictionData_Client() const
{
	if (!ClientPredictionData)
	{
		if (UPipPopMovementComponent* TempComponent = const_cast<UPipPopMovementComponent*>(this))
		{
			TempComponent->ClientPredictionData = new FPipPopNetworkPredictionData_Client_Character(*TempComponent);
		}
	}
	return ClientPredictionData;
}

void FPipPopSavedMove_Character::Clear()
{
	Super::Clear();
	bRequestingSprintSaved = false;
}

uint8 FPipPopSavedMove_Character::GetCompressedFlags() const
{
	uint8 Result = Super::GetCompressedFlags();
	
	if (bRequestingSprintSaved)
	{
		Result |= FLAG_Custom_0;
	}
	
	return Result;
}

void FPipPopSavedMove_Character::SetMoveFor(ACharacter* C, float InDeltaTime, FVector const& NewAccel,
	class FNetworkPredictionData_Client_Character& ClientData)
{
	Super::SetMoveFor(C, InDeltaTime, NewAccel, ClientData);
	if (C)
	{
		if (UPipPopMovementComponent* MovementComponent = Cast<UPipPopMovementComponent>(C->GetMovementComponent()))
		{
			bRequestingSprintSaved = MovementComponent->bRequestingSprint;
		}
	}
}

void FPipPopSavedMove_Character::PrepMoveFor(ACharacter* C)
{
	Super::PrepMoveFor(C);
	if (C)
	{
		if (UPipPopMovementComponent* MovementComponent = Cast<UPipPopMovementComponent>(C->GetMovementComponent()))
		{
			 MovementComponent->bRequestingSprint = bRequestingSprintSaved;
		}
	}
}

bool FPipPopSavedMove_Character::CanCombineWith(const FSavedMovePtr& NewMove, ACharacter* InCharacter,
	float MaxDelta) const
{
	if (bRequestingSprintSaved != static_cast<FPipPopSavedMove_Character*>(NewMove.Get())->bRequestingSprintSaved)
	{
		return false;
	}
	return Super::CanCombineWith(NewMove, InCharacter, MaxDelta);
}

FSavedMovePtr FPipPopNetworkPredictionData_Client_Character::AllocateNewMove()
{
	return FSavedMovePtr(new FPipPopSavedMove_Character);
}
