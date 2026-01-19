// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/MenuPawn.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputMappingContext.h"
#include "Interfaces/CustomisationInterface.h"

AMenuPawn::AMenuPawn()
{
	PrimaryActorTick.bCanEverTick = false;
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

void AMenuPawn::BeginPlay()
{
	Super::BeginPlay();
	if (APlayerController* PC = Cast<APlayerController>(GetController()))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PC->GetLocalPlayer()))
		{
			Subsystem->ClearAllMappings();
			Subsystem->AddMappingContext(InputMappingContext, 0);
		}
		PC->SetViewTargetWithBlend(this);
	}
}

void AMenuPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	if (UEnhancedInputComponent* PlayerEnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		if (InputActions.ClickAction)
		{
			PlayerEnhancedInputComponent->BindAction(InputActions.ClickAction, ETriggerEvent::Started, this, &AMenuPawn::OnLeftClick);
		}
	}
}

void AMenuPawn::OnLeftClick(const FInputActionValue& Value)
{
	if (const APlayerController* PlayerController = Cast<APlayerController>(GetController()))
	{
		FHitResult Hit(1.f);
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, "Shoot");
		PlayerController->GetHitResultUnderCursor(ECC_Visibility, true, Hit);
		if (Hit.bBlockingHit)
		{
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, "Score");
			constexpr float TraceDuration = 2.f;
			DrawDebugLine(GetWorld(), Hit.TraceStart, Hit.TraceEnd, FColor::Red, false, TraceDuration, 0, 0.1f);
			if (AActor* Actor = Hit.GetActor())
			{
				if (Actor->GetClass()->ImplementsInterface(UCustomisationInterface::StaticClass()))
				{
					if (UPrimitiveComponent* Component = Hit.GetComponent())
					{
						GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, "Do");
						ICustomisationInterface::Execute_Interact(Actor, Component);
					}
				}
			}
		}
	}
}
