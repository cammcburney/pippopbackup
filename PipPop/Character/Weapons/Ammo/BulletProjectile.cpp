// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Weapons/Ammo/BulletProjectile.h"
#include "Character/ShooterPlayerCharacter.h"
#include "Character/Components/CombatComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Net/UnrealNetwork.h"

// Sets default values
ABulletProjectile::ABulletProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;
	ProjectileMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ProjectileMeshComponent"));
	SetRootComponent(ProjectileMeshComponent);
	MovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("MovementComponent"));
	MovementComponent->InitialSpeed = 6000.0f;
	MovementComponent->MaxSpeed = 6000.0f;
	MovementComponent->ProjectileGravityScale = 1;
	MovementComponent->bShouldBounce = false;
	if (BulletMesh)
	{
		ProjectileMeshComponent->SetStaticMesh(BulletMesh);
		if (BulletMaterial)
		{
			ProjectileMeshComponent->SetMaterial(0, BulletMaterial);
		}
	}
}

// Called when the game starts or when spawned
void ABulletProjectile::BeginPlay()
{
	Super::BeginPlay();
	if (BulletMesh)
	{
		ProjectileMeshComponent->SetStaticMesh(BulletMesh);
		if (BulletMaterial)
		{
			ProjectileMeshComponent->SetMaterial(0, BulletMaterial);
		}
	}
	ProjectileMeshComponent->OnComponentHit.AddDynamic(this, &ABulletProjectile::ProjectileHit);
	SetActorRotation(FRotator(0, 0, 90));
}

void ABulletProjectile::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ABulletProjectile, Damage)
}

// Called every frame
void ABulletProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	FRotator Rotation = FRotator(0, 0, 360);
	AddActorLocalRotation(Rotation * DeltaTime);
}

void ABulletProjectile::ProjectileHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Red, TEXT("HIT"));
	GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::White, FString::Printf(TEXT("ACTOR HIT %s"), *HitComponent->GetName()));
	if (HasAuthority())
	{
		if (AShooterPlayerCharacter* HitPlayer = Cast<AShooterPlayerCharacter>(OtherActor))
		{
			if (UCombatComponent* CombatComponent = HitPlayer->GetCombatComponent())
			{
				CombatComponent->TakeDamage(Damage);
			}
		}
	}
	Destroy();
}


