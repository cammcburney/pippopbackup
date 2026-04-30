// Fill out your copyright notice in the Description page of Project Settings.


#include "Objects/Modifiers/GravityWarper.h"
#include "Components/ArrowComponent.h"
#include "Character/ShooterPlayerCharacter.h"
#include "Character/Components/PipPopMovementComponent.h"
#include "Components/SphereComponent.h"

// Sets default values
AGravityWarper::AGravityWarper()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	GravityRadiusComponent = CreateDefaultSubobject<USphereComponent>(TEXT("GravityRadiusComponent"));
	ArrowComponent = CreateDefaultSubobject<UArrowComponent>(TEXT("ArrowComponent"));
}

// Called when the game starts or when spawned
void AGravityWarper::BeginPlay()
{
	Super::BeginPlay();
	GravityRadiusComponent->OnComponentBeginOverlap.AddDynamic(this, &AGravityWarper::OnComponentOverlap);
	GravityRadiusComponent->OnComponentEndOverlap.AddDynamic(this, &AGravityWarper::EndComponentOverlap);
}

void AGravityWarper::OnComponentOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (AShooterPlayerCharacter* PlayerCharacter = Cast<AShooterPlayerCharacter>(OtherActor))
	{
		// if (UPipPopMovementComponent* MovementComponent = Cast<UPipPopMovementComponent>(PlayerCharacter->GetMovementComponent()))
		// {
		// 	if (ArrowComponent)
		// 	{
		// 		const FVector ArrowDirection = ArrowComponent->GetForwardVector();
		// 		MovementComponent->SetGravityDirection(ArrowDirection);
		// 	}
		// }
		PlayerCharacter->SetGravityCenter(GetActorLocation());
	}
}

void AGravityWarper::EndComponentOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
}

// Called every frame
void AGravityWarper::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

