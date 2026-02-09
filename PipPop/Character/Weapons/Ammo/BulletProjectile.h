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
	
private:

	UPROPERTY()
	TObjectPtr<UStaticMeshComponent> ProjectileMeshComponent;
	
	UPROPERTY()
	TObjectPtr<UProjectileMovementComponent> MovementComponent;
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
