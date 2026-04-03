// Fill out your copyright notice in the Description page of Project Settings.


#include "Objects/Dangerous/StaticDamager.h"
#include "Character/ShooterPlayerCharacter.h"
#include "Character/Components/CombatComponent.h"
#include "Components/CapsuleComponent.h"

// Sets default values
AStaticDamager::AStaticDamager()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComponent"));
	check(StaticMeshComponent);
	RootComponent = StaticMeshComponent;
	CollisionComponent = CreateDefaultSubobject<UCapsuleComponent>(TEXT("CollisionComponent"));
	check(CollisionComponent);
}

// Called when the game starts or when spawned
void AStaticDamager::BeginPlay()
{
	Super::BeginPlay();
	CollisionComponent->OnComponentBeginOverlap.AddDynamic(this, &AStaticDamager::OnOverlap);
	CollisionComponent->OnComponentEndOverlap.AddDynamic(this, &AStaticDamager::EndOverlap);
}

void AStaticDamager::OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	OverlappingActors.Add(OtherActor);
	SetActorTickEnabled(true);
}

void AStaticDamager::EndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	OverlappingActors.Remove(OtherActor);
}

// Called every frame
void AStaticDamager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (!OverlappingActors.Num())
	{
		SetActorTickEnabled(false);	
	}
	for (AActor* OverlappingActor : OverlappingActors)
	{
		if (AShooterPlayerCharacter* PlayerCharacter = Cast<AShooterPlayerCharacter>(OverlappingActor))
		{
			if (UCombatComponent* CombatComponent = PlayerCharacter->GetCombatComponent())
			{
				CombatComponent->TakeDamage(Damage);
			}
		}
	}
}

