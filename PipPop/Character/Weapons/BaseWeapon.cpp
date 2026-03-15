// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Weapons/BaseWeapon.h"
#include "Camera/CameraComponent.h"
#include "Ammo/BulletProjectile.h"

// Sets default values
ABaseWeapon::ABaseWeapon()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;
	SkeletalMeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SkeletalMeshComponent"));
	check(SkeletalMeshComponent);
	SkeletalMeshComponent->SetupAttachment(RootComponent);
	SightsCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("SightsCamera"));
	check(SightsCamera);
	SightsCamera->SetupAttachment(SkeletalMeshComponent, FName("Sights"));
	SightsCamera->SetRelativeLocationAndRotation(FVector(30.f, 0.f, 25.f), FRotator::ZeroRotator);
	SightsCamera->bUsePawnControlRotation = true;
	SightsCamera->bEnableFirstPersonFieldOfView = true;
	SightsCamera->bEnableFirstPersonScale = true;
	SightsCamera->FirstPersonFieldOfView = AimFieldOfView;
	SightsCamera->FirstPersonScale = 0.6;
	SightsCamera->SetAutoActivate(false);
}

// Called when the game starts or when spawned
void ABaseWeapon::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void ABaseWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ABaseWeapon::Aim_Implementation()
{
	bIsAiming = true;
}

bool ABaseWeapon::Aim_Validate()
{
	return true;
}

void ABaseWeapon::ClientAim()
{
	SightsCamera->Activate();
	if (APlayerController* PlayerController = Cast<APlayerController>(GetInstigatorController()))
	{
		PlayerController->SetViewTargetWithBlend(this, .5f);
	}
}

void ABaseWeapon::StopAim_Implementation()
{
	bIsAiming = false;
}

void ABaseWeapon::ClientStopAiming()
{
	SightsCamera->Deactivate();
}

bool ABaseWeapon::StopAim_Validate()
{
	return true;
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
	GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Red, FString::Printf(TEXT("bIsAiming %s"), bIsAiming ? TEXT("true") : TEXT("false")));
	FRotator SocketRotation = SkeletalMeshComponent->GetSocketRotation("Muzzle");
	FRotator SpreadRotation = SocketRotation;
	float SpreadMultiplier = bIsAiming ? AimingSpreadReduction : 1.f;
	SpreadRotation.Pitch += FMath::RandRange(-SpreadAngles.DownAngle, SpreadAngles.UpAngle) * SpreadMultiplier; 
	SpreadRotation.Yaw += FMath::RandRange(-SpreadAngles.LeftAngle, SpreadAngles.RightAngle) * SpreadMultiplier;  
	const FVector SpreadVector = SpreadRotation.Vector();
	const FVector MuzzleLocation = SkeletalMeshComponent->GetSocketLocation("Muzzle");
	const FVector Start = MuzzleLocation + SpreadVector;
	
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
