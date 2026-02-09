// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Weapons/Ammo/BulletProjectile.h"
#include "GameFramework/ProjectileMovementComponent.h"
// Sets default values
ABulletProjectile::ABulletProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;
	ProjectileMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ProjectileMeshComponent"));
	SetRootComponent(ProjectileMeshComponent);
	MovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("MovementComponent"));
	MovementComponent->InitialSpeed = 3000.0f;
	MovementComponent->MaxSpeed = 3000.0f;
	MovementComponent->bShouldBounce = false;
	if (BulletMesh)
	{
		ProjectileMeshComponent->SetStaticMesh(BulletMesh);
	}
}

// Called when the game starts or when spawned
void ABulletProjectile::BeginPlay()
{
	Super::BeginPlay();
	if (BulletMesh)
	{
		ProjectileMeshComponent->SetStaticMesh(BulletMesh);
	}
}

// Called every frame
void ABulletProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

