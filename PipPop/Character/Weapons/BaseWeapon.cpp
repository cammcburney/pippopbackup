// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Weapons/BaseWeapon.h"
#include "Ammo/BulletProjectile.h"

// Sets default values
ABaseWeapon::ABaseWeapon()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;
	SkeletalMeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SkeletalMeshComponent"));
	SkeletalMeshComponent->SetupAttachment(RootComponent);
	if (SkeletalMesh)
	{
		SkeletalMeshComponent->SetSkeletalMesh(SkeletalMesh);
	}
}

// Called when the game starts or when spawned
void ABaseWeapon::BeginPlay()
{
	Super::BeginPlay();
	if (SkeletalMesh)
	{
		SkeletalMeshComponent->SetSkeletalMesh(SkeletalMesh);
	}
}

// Called every frame
void ABaseWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ABaseWeapon::Fire_Implementation()
{
	if (ProjectileClass)
	{
		if (const UWorld* World = GetWorld())
		{
			bFiring = true;

			const float TimeSinceLastShot = World->GetTimeSeconds() - LastFireTime;
			const float RemainingTime = RateOfFire - TimeSinceLastShot;
			if (TimeSinceLastShot > RateOfFire)
			{
				for (int32 i = 0; i < BulletsFiredPerShot; i++)
				{
					FireProjectile();
				}
			}
			else
			{
				for (int32 i = 0; i < BulletsFiredPerShot; i++)
				{
					if (bIsAutomatic)
					{
						World->GetTimerManager().SetTimer(FiringTimerHandle, this, &ABaseWeapon::FireProjectile, RemainingTime, false);
					}
				}
			}
		}
	}
}

bool ABaseWeapon::Fire_Validate()
{
	return true;
}

FBulletTrajectory ABaseWeapon::CalculateBulletTrajectory(const FVector& Location, const FRotator& Rotation)
{
	FRotator SpreadRotation = Rotation;
	SpreadRotation.Pitch += FMath::RandRange(-SpreadAngles.DownAngle, SpreadAngles.UpAngle); 
	SpreadRotation.Yaw += FMath::RandRange(-SpreadAngles.LeftAngle, SpreadAngles.RightAngle);    
	const FVector SpreadVector = SpreadRotation.Vector();
	const FVector Start = Location + FVector(200, 0, 50) + SpreadVector;
	const FRotator SpreadRotator = SpreadRotation.GetEquivalentRotator();
	const  FRotator ShotDirection = SpreadRotator;
	return FBulletTrajectory{ ShotDirection, Start };
}

void ABaseWeapon::FireProjectile()
{
	if (UWorld* World = GetWorld())
	{
		LastFireTime = World->GetTimeSeconds();
		const AController* PlayerController = GetOwner()->GetInstigatorController();
		const FBulletTrajectory BulletTrajectory = CalculateBulletTrajectory(GetActorLocation(), PlayerController->GetControlRotation());
		FActorSpawnParameters SpawnParameters;
		SpawnParameters.Instigator = Cast<APawn>(GetOwner());
		SpawnParameters.Owner = this;
		SpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		SpawnParameters.TransformScaleMethod = ESpawnActorScaleMethod::OverrideRootScale;
		World->SpawnActor<ABulletProjectile>(ProjectileClass, BulletTrajectory.Start, BulletTrajectory.ShotDirection, SpawnParameters);
	}
}
