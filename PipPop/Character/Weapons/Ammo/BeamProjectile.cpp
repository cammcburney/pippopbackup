// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Weapons/Ammo/BeamProjectile.h"
#include "Character/Weapons/BaseWeapon.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraComponent.h"

void ABeamProjectile::TriggerNiagaraSystem(ABaseWeapon* OwningWeapon, FBulletTrajectory BulletTrajectory)
{
	if (OwningWeapon)
	{
		if (USkeletalMeshComponent* OwningWeaponMesh = OwningWeapon->SkeletalMeshComponent)
		{
			NiagaraComponent = UNiagaraFunctionLibrary::SpawnSystemAttached(
				ProjectileNiagaraSystem,
				OwningWeaponMesh,
				"Muzzle",
				BulletTrajectory.Start,
				BulletTrajectory.ShotDirection,
				EAttachLocation::Type::KeepRelativeOffset,
				true);
			NiagaraComponent->SetVectorParameter(FName("BeamStart"), BulletTrajectory.Start);
			NiagaraComponent->SetVectorParameter(FName("BeamEnd"), ProjectileMeshComponent->GetComponentLocation());
			GetWorld()->GetTimerManager().SetTimer(BeamTimer, this, &ABeamProjectile::UpdateBeam, 0.01f, true);
		}
	}
}

void ABeamProjectile::DestroyParticle()
{
	GetWorld()->GetTimerManager().ClearTimer(BeamTimer);
	if (NiagaraComponent)
	{
		NiagaraComponent->Deactivate();
	}
	Super::DestroyParticle();
}

void ABeamProjectile::UpdateBeam()
{
	if (NiagaraComponent && ProjectileMeshComponent)
	{
		NiagaraComponent->SetVectorParameter(FName("BeamEnd"), ProjectileMeshComponent->GetComponentLocation());
	}
}
