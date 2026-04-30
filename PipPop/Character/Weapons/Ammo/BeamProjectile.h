// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/Weapons/Ammo/BulletProjectile.h"
#include "BeamProjectile.generated.h"

class UNiagaraComponent;
class UNiagaraFunctionLibrary;
/**
 * 
 */
UCLASS(Blueprintable)
class PIPPOP_API ABeamProjectile : public ABulletProjectile
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere)
	FName BeamStartName = "BeamStart";

	UPROPERTY(EditAnywhere)
	FName BeamEndName = "BeamEnd";
	
	FTimerHandle BeamTimer;

	virtual void TriggerNiagaraSystem(ABaseWeapon* OwningWeapon, FBulletTrajectory BulletTrajectory) override;

	virtual void DestroyParticle() override;
	
	virtual void UpdateBeam();
	
};
