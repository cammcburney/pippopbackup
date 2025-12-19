// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/CustomisationPawn.h"
#include "Camera/CameraComponent.h"
#include "Components/AppearanceComponent.h"
#include "GameFramework/SpringArmComponent.h"

// Sets default values
ACustomisationPawn::ACustomisationPawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	AppearanceComponent = CreateDefaultSubobject<UAppearanceComponent>("AppearanceComponent");
	SpringArm = CreateDefaultSubobject<USpringArmComponent>("SpringArm");
	if (SpringArm)
	{
		SpringArm->SetupAttachment(RootComponent);
		SpringArm->TargetArmLength = 350.f;
	}
	CameraComponent = CreateDefaultSubobject<UCameraComponent>("CameraComponent");
	if (CameraComponent)
	{
		CameraComponent->SetupAttachment(SpringArm);
		CameraComponent->SetRelativeRotation(FRotator(0, -180.f, 0));
	}

}

// Called when the game starts or when spawned
void ACustomisationPawn::BeginPlay()
{
	Super::BeginPlay();
	if (APlayerController* PC = Cast<APlayerController>(GetController()))
	{
		GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::White, TEXT("Set view target"));
		PC->SetViewTargetWithBlend(this);
	}
}

// Called every frame
void ACustomisationPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ACustomisationPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

