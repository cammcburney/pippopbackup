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
		if (UWorld* World = GetWorld())
		{
			FActorSpawnParameters SpawnParameters;
			SpawnParameters.Instigator = Cast<APawn>(GetOwner());
			SpawnParameters.Owner = this;
			SpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
			SpawnParameters.TransformScaleMethod = ESpawnActorScaleMethod::OverrideRootScale;
			World->SpawnActor<ABulletProjectile>(ProjectileClass, GetActorLocation() + FVector(100, 0, 50), GetActorRotation(), SpawnParameters);
		}
	}
}

bool ABaseWeapon::Fire_Validate()
{
	return true;
}

