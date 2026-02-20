// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Weapons/Ammo/BulletProjectile.h"
#include "Character/ShooterPlayerCharacter.h"
#include "Character/Components/CombatComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"
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
	MovementComponent->InitialSpeed = InitVelocity * 100.f;
	MovementComponent->MaxSpeed = MaxSpeed * 100.f;
	MovementComponent->ProjectileGravityScale = GravityScale;
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

	SpawnTime = GetWorld()->GetTimeSeconds();
	SpawnLocation = GetActorLocation();
	MovementComponent->Velocity = GetActorForwardVector() * (InitVelocity * 100.f);
	CurveDirection = FVector(
		FMath::RandRange(-1.f, 1.f),
		FMath::RandRange(-1.f, 1.f),
		FMath::RandRange(-1.f, 1.f)
	).GetSafeNormal();
	if (ProjectileMeshComponent)
	{
		// ProjectileMeshComponent->SetCollisionProfileName(FName("Projectile"));
		if (BulletMesh)
		{
			ProjectileMeshComponent->SetStaticMesh(BulletMesh);
			if (BulletMaterial)
			{
				ProjectileMeshComponent->SetMaterial(0, BulletMaterial);
			}
		}
	}
	ProjectileMeshComponent->OnComponentHit.AddDynamic(this, &ABulletProjectile::ProjectileHit);
	GetWorld()->GetTimerManager().SetTimer(DestroyTimer, this, &ABulletProjectile::DestroyParticle, Lifetime, false);
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
	AddActorLocalRotation(FRotator(0, 0, 360) * DeltaTime);
	if (MovementComponent)
	{
		const float LifetimePercent = FMath::Clamp((GetWorld()->GetTimeSeconds() - SpawnTime) / Lifetime, 0.f, 1.f);
		float CurveIntensity = CurveIntensity = FMath::Pow(LifetimePercent, CurveExponent);
		if (BulletCurve)
		{
			CurveIntensity = BulletCurve->GetFloatValue(LifetimePercent);
		}
		MovementComponent->Velocity += CurveDirection.GetSafeNormal() * (CurveMaxStrength * 100) * CurveIntensity * DeltaTime;
	}
	
}

void ABulletProjectile::ProjectileHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	if (HasAuthority())
	{
		if (AShooterPlayerCharacter* HitPlayer = Cast<AShooterPlayerCharacter>(OtherActor))
		{
			if (UCombatComponent* CombatComponent = HitPlayer->GetCombatComponent())
			{
				CombatComponent->TakeDamage(Damage);
			}
		}
		DestroyParticle();
	}
	
}

void ABulletProjectile::DestroyParticle()
{
	if (HasAuthority())
	{
		Destroy();
	}
	GetWorld()->GetTimerManager().ClearTimer(DestroyTimer);
}

