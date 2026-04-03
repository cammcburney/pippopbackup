// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Pawns/Spectator/StaticSpectatorPawn.h"
#include "Mode/Gameplay/ShooterMode.h"

// Sets default values
AStaticSpectatorPawn::AStaticSpectatorPawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->SetupAttachment(RootComponent);
	SpringArm->SetRelativeLocationAndRotation(FVector(0.0f, 0.0f, 70.0f), FRotator(-25.0f, -0.f, 0.0f));
	SpringArm->TargetArmLength = 430.f;
	SpringArm->bEnableCameraLag = true;
	SpringArm->CameraLagSpeed = 6.0f;
	SpringArm->bDrawDebugLagMarkers = true;
	MainCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraMain"));
	MainCamera->SetupAttachment(SpringArm, USpringArmComponent::SocketName);

}

void AStaticSpectatorPawn::RespawnPlayer()
{
	SpawnTimer.Invalidate();
	if (APlayerController* PlayerController = Cast<APlayerController>(GetController()))
	{
		PlayerController->UnPossess();
		if (AShooterMode* GameMode = Cast<AShooterMode>(GetWorld()->GetAuthGameMode()))
		{
			GameMode->SpawnPlayer(PlayerController);
		}
	}
	Destroy();
}

// Called when the game starts or when spawned
void AStaticSpectatorPawn::BeginPlay()
{
	Super::BeginPlay();
	GetWorld()->GetTimerManager().SetTimer(SpawnTimer, this, &AStaticSpectatorPawn::RespawnPlayer, RespawnTime, false);
}

// Called every frame
void AStaticSpectatorPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (SpringArm)
	{
		FRotator CurrentRotation = SpringArm->GetComponentRotation();
		CurrentRotation.Yaw += 25.f * DeltaTime;
		SpringArm->SetRelativeRotation(CurrentRotation);
	}
	
}

// Called to bind functionality to input
void AStaticSpectatorPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

