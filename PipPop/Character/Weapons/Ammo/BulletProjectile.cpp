// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Weapons/Ammo/BulletProjectile.h"
#include "Character/ShooterPlayerCharacter.h"
#include "Character/Components/CombatComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraComponent.h"
#include "NiagaraSystem.h"
#include "Character/Weapons/BaseWeapon.h"
#include "Components/AudioComponent.h"


// Sets default values
ABulletProjectile::ABulletProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;
	ProjectileMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ProjectileMeshComponent"));
	check(ProjectileMeshComponent)
	SetRootComponent(ProjectileMeshComponent);
	
	MovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("MovementComponent"));
	check(MovementComponent)
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

	ProjectileNiagaraSystem = CreateDefaultSubobject<UNiagaraSystem>(TEXT("ProjectileNiagaraSystem"));
	check(ProjectileNiagaraSystem);

	NiagaraComponent = CreateDefaultSubobject<UNiagaraComponent>(TEXT("BeamComponent"));
	check(NiagaraComponent);

	BulletSound = CreateDefaultSubobject<USoundBase>(TEXT("BulletSound"));
}

// Called when the game starts or when spawned
void ABulletProjectile::BeginPlay()
{
	MovementComponent->InitialSpeed = InitVelocity * 100.f;
	MovementComponent->MaxSpeed = MaxSpeed * 100.f;
	MovementComponent->Velocity = GetActorForwardVector() * (InitVelocity * 100.f);
	Super::BeginPlay();
	SpawnTime = GetWorld()->GetTimeSeconds();
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
	SpawnBulletSound();
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
	// AddActorLocalRotation(FRotator(0, 0, 360) * DeltaTime);
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

void ABulletProjectile::TriggerNiagaraSystem(ABaseWeapon* OwningWeapon, FBulletTrajectory BulletTrajectory)
{
}

void ABulletProjectile::SpawnBulletSound()
{
	if (BulletSound)
	{
		USoundAttenuation* SoundAttenuation = NewObject<USoundAttenuation>(this);
		SoundAttenuation->Attenuation.bSpatialize = true;
		SoundAttenuation->Attenuation.bAttenuate = true;
		SoundAttenuation->Attenuation.AttenuationShape = EAttenuationShape::Sphere;
		SoundAttenuation->Attenuation.AttenuationShapeExtents = FVector(300.f);
		SoundAttenuation->Attenuation.FalloffDistance = 15000.f;
		SoundAttenuation->Attenuation.dBAttenuationAtMax = -80.f;
		SoundAttenuation->Attenuation.DistanceAlgorithm = EAttenuationDistanceModel::Logarithmic;
		BulletAudioComponent = UGameplayStatics::SpawnSoundAttached(
			BulletSound,
			ProjectileMeshComponent,
			NAME_None,
			FVector::ZeroVector,
			EAttachLocation::KeepRelativeOffset,
			true,
			1.f,
			1.f,
			0.f,
			SoundAttenuation);
	}
}

void ABulletProjectile::DestroyParticle()
{
	if (BulletAudioComponent)
	{
		BulletAudioComponent->FadeOut(.5f, 0.f, EAudioFaderCurve::Linear);
	}
	if (HasAuthority())
	{
		GetWorld()->GetTimerManager().ClearTimer(DestroyTimer);
	}
	ProjectileMeshComponent->DestroyComponent();
	FTimerHandle TimerHandle;
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &ABulletProjectile::DestroyProjectile, 1.f, false);
}

void ABulletProjectile::DestroyProjectile()
{
	Destroy();
}