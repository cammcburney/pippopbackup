// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BaseWeapon.generated.h"

class ABulletProjectile;

UCLASS(Blueprintable)
class PIPPOP_API ABaseWeapon : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABaseWeapon();

	UPROPERTY(EditAnywhere)
	USkeletalMesh* SkeletalMesh;

	UPROPERTY(EditAnywhere)
	TSubclassOf<ABulletProjectile> ProjectileClass;
	
private:

	UPROPERTY()
	USkeletalMeshComponent* SkeletalMeshComponent;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(Server, Reliable, WithValidation)
	virtual void Fire();
	virtual void Fire_Implementation();
	virtual bool Fire_Validate();
	
};
