// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/CustomisationPawn.h"
#include "Camera/CameraComponent.h"
#include "Components/AppearanceComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputMappingContext.h"
#include "Interfaces/CustomisationInterface.h"

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
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PC->GetLocalPlayer()))
		{
			Subsystem->ClearAllMappings();
			Subsystem->AddMappingContext(InputMappingContext, 0);
		}
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
	// Make sure that we are using a UEnhancedInputComponent; if not, the project is not configured correctly.
	if (UEnhancedInputComponent* PlayerEnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		// There are ways to bind a UInputAction* to a handler function and multiple types of ETriggerEvent that may be of interest.
 
		// This calls the handler function on the tick when MyInputAction starts, such as when pressing an action button.
		if (InputActions.ClickAction)
		{
			PlayerEnhancedInputComponent->BindAction(InputActions.ClickAction, ETriggerEvent::Started, this, &ACustomisationPawn::OnLeftClick);
		}
	}
}

void ACustomisationPawn::OnLeftClick(const FInputActionValue& Value)
{
	// if (UWorld* World = GetWorld())
	// {
	// 	FHitResult Hit(1.f);
	// 	constexpr float TraceDistance = 500.f; constexpr float TraceDuration = 5.f;
	// 	const FVector TraceStart = GetActorLocation();
	// 	const FVector TraceEnd = TraceStart + GetActorForwardVector() * TraceDistance;
	// 	FCollisionQueryParams CollisionParams;
	// 	CollisionParams.AddIgnoredActor(this);
	// 	bool bHit = World->LineTraceSingleByChannel(Hit, TraceStart, TraceEnd, ECC_Visibility, CollisionParams);
	// 	DrawDebugLine(World, TraceStart, TraceEnd, FColor::Red, false, TraceDuration, 0, TraceDuration);
	// }
	if (const APlayerController* PlayerController = Cast<APlayerController>(GetController()))
	{
		constexpr float TraceDuration = 2.f;
		FHitResult Hit(1.f);
		PlayerController->GetHitResultUnderCursor(ECC_Visibility, true, Hit);
		if (Hit.bBlockingHit)
		{
			DrawDebugLine(GetWorld(), Hit.TraceStart, Hit.TraceEnd, FColor::Red, false, TraceDuration, 0, 0.1f);
			if (AActor* Actor = Hit.GetActor())
			{
				if (Actor->GetClass()->ImplementsInterface(UCustomisationInterface::StaticClass()))
				{
					if (UPrimitiveComponent* Component = Hit.GetComponent())
					{
						ICustomisationInterface::Execute_Interact(Actor, Component);
					}
				}
			}
		}
		
	}
}


