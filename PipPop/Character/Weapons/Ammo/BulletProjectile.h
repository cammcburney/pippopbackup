// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BulletProjectile.generated.h"

class UProjectileMovementComponent;

UCLASS()
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
	
private:

	UPROPERTY()
	TObjectPtr<UStaticMeshComponent> ProjectileMeshComponent;
	
	UPROPERTY()
	TObjectPtr<UProjectileMovementComponent> MovementComponent;

public:
	
	UFUNCTION()
	virtual void ProjectileHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);
	
protected:

	virtual void BeginPlay() override;

	
	
public:	
	
	virtual void Tick(float DeltaTime) override;

	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;

};
