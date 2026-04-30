// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BaseWeapon.generated.h"

class ABulletProjectile;
class UCameraComponent;

USTRUCT(BlueprintType)
struct FBulletTrajectory
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Bullet")
	FRotator ShotDirection;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Bullet")
	FVector Start;
};

USTRUCT(BlueprintType)
struct FSpreadAngleAxis
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	float UpAngle; 
	UPROPERTY(EditAnywhere)
	float DownAngle; 
	UPROPERTY(EditAnywhere)
	float LeftAngle; 
	UPROPERTY(EditAnywhere)
	float RightAngle; 
};

UCLASS(Blueprintable)
class PIPPOP_API ABaseWeapon : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABaseWeapon();
	
	UPROPERTY(EditAnywhere)
	TSubclassOf<ABulletProjectile> ProjectileClass;
	
	UPROPERTY(EditAnywhere)
	bool bIsAutomatic;

	UPROPERTY(EditAnywhere)
	float RateOfFire = 0.2f;

	UPROPERTY(EditAnywhere)
	FSpreadAngleAxis SpreadAngles;

	UPROPERTY(EditAnywhere)
	int32 BulletsFiredPerShot = 5;

	UPROPERTY(VisibleAnywhere, Category = Camera)
	TObjectPtr<UCameraComponent> SightsCamera;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TObjectPtr<USkeletalMeshComponent> SkeletalMeshComponent;

	UPROPERTY(EditDefaultsOnly)
	float AimFieldOfView = 70.f;

	UPROPERTY(EditDefaultsOnly)
	float AimingSpreadReduction = .7f;
	
private:

	bool bFiring = false;
	
	float LastFireTime = 0.0f;
	
	FTimerHandle FiringTimerHandle;
	
	FBulletTrajectory CalculateBulletTrajectory(const FVector& Location, const FRotator& Rotation);

	bool bIsAiming = false;
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(Server, Reliable, WithValidation)
	virtual void Aim();
	virtual void Aim_Implementation();
	virtual bool Aim_Validate();
	
	virtual void ClientAim();
	
	UFUNCTION(Server, Reliable, WithValidation)
	virtual void StopAim();
	virtual void StopAim_Implementation();
	virtual bool StopAim_Validate();

	virtual void ClientStopAiming();
	
	UFUNCTION(Server, Reliable, WithValidation)
	virtual void Fire();
	virtual void Fire_Implementation();
	virtual bool Fire_Validate();

	void FireProjectile();
	
};
