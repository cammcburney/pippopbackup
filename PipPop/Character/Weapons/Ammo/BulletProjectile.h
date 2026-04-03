// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BulletProjectile.generated.h"

class UProjectileMovementComponent;

UCLASS(Blueprintable)
class PIPPOP_API ABulletProjectile : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABulletProjectile();

	UPROPERTY(EditAnywhere)
	TObjectPtr<UStaticMesh> BulletMesh;

	UPROPERTY(EditAnywhere)
	TObjectPtr<UMaterialInterface> BulletMaterial;

	UPROPERTY(EditAnywhere, Replicated)
	float Damage = 10;

	UPROPERTY(EditAnywhere, meta=(ClampMin=1, ClampMax=10000))
	float InitVelocity = 10000.f;

	UPROPERTY(EditAnywhere, meta=(ClampMin=1, ClampMax=10000))
	float MaxSpeed = 10000.f;
	
	UPROPERTY(EditAnywhere)
	float Lifetime = 2.f;

	UPROPERTY(EditAnywhere, meta=(ClampMin=-100, ClampMax=100))
	float GravityScale = .1f;
	FTimerHandle DestroyTimer;

	// Randomised in begin play by default
	UPROPERTY(EditAnywhere)
	FVector CurveDirection;

	// How much the bullet will curve 
	UPROPERTY(EditAnywhere)
	float CurveMaxStrength = 50.f;

	// Curve exponent, higher values for a later more intense curve, lower for earlier more constant curve
	UPROPERTY(EditAnywhere)
	float CurveExponent = 1.5f;
	
	UPROPERTY(EditAnywhere)
	TObjectPtr<UCurveFloat> BulletCurve;

private:

	UPROPERTY(EditAnywhere)
	TObjectPtr<UStaticMeshComponent> ProjectileMeshComponent;
	
	UPROPERTY()
	TObjectPtr<UProjectileMovementComponent> MovementComponent;

	float SpawnTime;
	
public:
	
	UFUNCTION()
	virtual void ProjectileHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);
	
protected:

	virtual void BeginPlay() override;

public:	
	
	virtual void Tick(float DeltaTime) override;

	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;

	virtual void DestroyParticle();
};
