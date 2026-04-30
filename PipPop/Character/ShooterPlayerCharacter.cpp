// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/ShooterPlayerCharacter.h"
#include "Controller/ShooterController.h"
#include "Components/CombatComponent.h"
#include "Camera/CameraComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Components/PipPopMovementComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Net/UnrealNetwork.h"
#include "Pawns/Spectator/StaticSpectatorPawn.h"
#include "Weapons/BaseWeapon.h"


// Sets default values
AShooterPlayerCharacter::AShooterPlayerCharacter(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer.SetDefaultSubobjectClass<UPipPopMovementComponent>(ACharacter::CharacterMovementComponentName))
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;
	SetReplicates(true);
	bUseControllerRotationPitch = true;
	bUseControllerRotationYaw = true;
	bUseControllerRotationRoll = true;
	FirstPersonCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCameraComponent"));
	check(FirstPersonCameraComponent);
	FirstPersonCameraComponent->SetupAttachment(GetMesh());
	FirstPersonCameraComponent->SetRelativeLocation(FVector(30.f, 0.f, 25.f));
	FirstPersonCameraComponent->bUsePawnControlRotation = true;
	FirstPersonCameraComponent->bEnableFirstPersonFieldOfView = true;
	FirstPersonCameraComponent->bEnableFirstPersonScale = true;
	FirstPersonCameraComponent->FirstPersonFieldOfView = 90.f;
	FirstPersonCameraComponent->FirstPersonScale = 0.6;
	CombatComponent = CreateDefaultSubobject<UCombatComponent>(TEXT("CombatComponent"));
	check(CombatComponent);
	PrimaryWeapon = CreateDefaultSubobject<ABaseWeapon>(TEXT("PrimaryWeaponClass"));
	check(PrimaryWeapon)
	GetCharacterMovement()->JumpZVelocity = 850.f;
	GetCharacterMovement()->GravityScale = 3.f;
	GetCharacterMovement()->MaxWalkSpeed = 1200.f;
}

// Called when the game starts or when spawned
void AShooterPlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
	if (HasAuthority())
	{
		if (UWorld* World = GetWorld())
		{
			if (!PrimaryWeaponClass) {return;}
			FActorSpawnParameters SpawnParameters;
			SpawnParameters.Instigator = this;
			SpawnParameters.Owner = this;
			if (ABaseWeapon* Weapon = World->SpawnActor<ABaseWeapon>(PrimaryWeaponClass, GetActorLocation(), FRotator::ZeroRotator, SpawnParameters))
			{
				EquippedWeapon = Weapon;
				Weapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, FName("Hand"));
			}
		}
	}
}

// Called every frame
void AShooterPlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (!GravityCenter.IsNearlyZero())
	{
		if (UPipPopMovementComponent* MovementComponent = Cast<UPipPopMovementComponent>(GetCharacterMovement()))
		{
			// FRotator LookRotation = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), GravityCenter);
			// MovementComponent->SetGravityDirection(LookRotation.Vector());
			const FVector GravityDirection = -(GetActorLocation() - GravityCenter).GetSafeNormal();
			MovementComponent->SetGravityDirection(GravityDirection);
			
			FVector ProjectedForward = FVector::VectorPlaneProject(GetActorForwardVector(), -GravityDirection).GetSafeNormal();
			FQuat TargetRotation = FRotationMatrix::MakeFromXZ(-GravityDirection, ProjectedForward).ToQuat();
			FQuat FinalRotation = FQuat::Slerp(GetActorQuat(), TargetRotation, FMath::Clamp(DeltaTime * 8.f, 0.f, 1.f));
			SetActorRotation(FinalRotation);
		}
	}
}

void AShooterPlayerCharacter::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AShooterPlayerCharacter, CombatComponent);
	DOREPLIFETIME(AShooterPlayerCharacter, EquippedWeapon);
}

// Called to bind functionality to input
void AShooterPlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent))
	{
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AShooterPlayerCharacter::PlayerMove);
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AShooterPlayerCharacter::PlayerLook);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &AShooterPlayerCharacter::PlayerJump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &AShooterPlayerCharacter::PlayerStopJump);
		EnhancedInputComponent->BindAction(ChatAction, ETriggerEvent::Started, this,  &AShooterPlayerCharacter::PlayerChat);
		EnhancedInputComponent->BindAction(AimAction, ETriggerEvent::Started, this,  &AShooterPlayerCharacter::PlayerAim);
		EnhancedInputComponent->BindAction(AimAction, ETriggerEvent::Completed, this , &AShooterPlayerCharacter::PlayerStopAiming);
		EnhancedInputComponent->BindAction(FireAction, ETriggerEvent::Triggered, this, &AShooterPlayerCharacter::PlayerFire);
		EnhancedInputComponent->BindAction(SprintAction, ETriggerEvent::Started, this, &AShooterPlayerCharacter::PlayerSprint);
		EnhancedInputComponent->BindAction(SprintAction, ETriggerEvent::Completed, this, &AShooterPlayerCharacter::PlayerStopSprinting);
		EnhancedInputComponent->BindAction(SlideAction, ETriggerEvent::Started, this,  &AShooterPlayerCharacter::PlayerSlide);
		EnhancedInputComponent->BindAction(SlideAction, ETriggerEvent::Completed, this,  &AShooterPlayerCharacter::PlayerStopSliding);
	}
}

void AShooterPlayerCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);
	SetupInputMapping();
}

void AShooterPlayerCharacter::OnRep_Controller()
{
	Super::OnRep_Controller();
	SetupInputMapping();
}

void AShooterPlayerCharacter::SetupInputMapping()
{
	if (const APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(MappingContext, 0);
		}
	}
}

void AShooterPlayerCharacter::PlayerMove(const FInputActionValue& Value)
{
	const FVector2D MovementValue = Value.Get<FVector2D>();
	if (UPipPopMovementComponent* MovementComponent = Cast<UPipPopMovementComponent>(GetMovementComponent()))
	{
		if (Controller && FirstPersonCameraComponent)
		{
			const FVector GravityDirection = MovementComponent->GetGravityDirection();
			const FVector CameraRightDirection = FirstPersonCameraComponent->GetRightVector();
			const FVector Right = CameraRightDirection - GravityDirection * FVector::DotProduct(GravityDirection, CameraRightDirection);
			AddMovementInput(Right.GetSafeNormal(), MovementValue.X);
			const FVector CameraForwardDirection = FirstPersonCameraComponent->GetForwardVector();
			const FVector Forward = CameraForwardDirection - GravityDirection * FVector::DotProduct(GravityDirection, CameraForwardDirection);
			AddMovementInput(Forward, MovementValue.Y);
		}
	}
}

void AShooterPlayerCharacter::PlayerLook(const FInputActionValue& Value)
{
	const FVector2D MovementValue = Value.Get<FVector2D>();
	if (Controller)
	{
		AddControllerYawInput(MovementValue.X);
		AddControllerPitchInput(MovementValue.Y);
	}
}

void AShooterPlayerCharacter::PlayerJump(const FInputActionValue& Value)
{
	bPressedJump = true;
	if (UPipPopMovementComponent* MovementComponent = Cast<UPipPopMovementComponent>(GetMovementComponent()))
	{
		MovementComponent->WallJump();
	}
	
}

void AShooterPlayerCharacter::PlayerStopJump(const FInputActionValue& Value)
{
	bPressedJump = false;
}

void AShooterPlayerCharacter::PlayerSprint(const FInputActionValue& Value)
{
	if (UPipPopMovementComponent* MovementComponent = Cast<UPipPopMovementComponent>(GetCharacterMovement()))
	{
		MovementComponent->SetSprinting(true);
	}
}

void AShooterPlayerCharacter::PlayerStopSprinting()
{
	if (UPipPopMovementComponent* MovementComponent = Cast<UPipPopMovementComponent>(GetCharacterMovement()))
	{
		MovementComponent->SetSprinting(false);
	}
}

void AShooterPlayerCharacter::PlayerSlide()
{
	if (UPipPopMovementComponent* MovementComponent = Cast<UPipPopMovementComponent>(GetCharacterMovement()))
	{
		MovementComponent->Slide();
	}
}

void AShooterPlayerCharacter::PlayerStopSliding()
{
	if (UPipPopMovementComponent* MovementComponent = Cast<UPipPopMovementComponent>(GetCharacterMovement()))
	{
		MovementComponent->StopSliding();
	}
}

void AShooterPlayerCharacter::PlayerChat()
{
	if (AShooterController* PlayerController = Cast<AShooterController>(Controller))
	{
		PlayerController->FocusChat();
	}
}

void AShooterPlayerCharacter::PlayerAim()
{
	if (FirstPersonCameraComponent)
	{
		FirstPersonCameraComponent->Deactivate();
	}
	if (EquippedWeapon)
	{
		EquippedWeapon->Aim();
		EquippedWeapon->ClientAim();
	}
}

void AShooterPlayerCharacter::PlayerStopAiming()
{
	if (FirstPersonCameraComponent)
	{
		FirstPersonCameraComponent->Activate();
	}
	if (EquippedWeapon)
	{
		EquippedWeapon->StopAim();
		EquippedWeapon->ClientStopAiming();
		if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
		{
			PlayerController->SetViewTargetWithBlend(this, .3f, VTBlend_Linear, 0, true);
		}
	}
}

void AShooterPlayerCharacter::PlayerFire(const FInputActionValue& Value)
{
	const bool FiringValue = Value.Get<bool>();
	if (EquippedWeapon && FiringValue)
	{
		EquippedWeapon->Fire();
	}
}

void AShooterPlayerCharacter::PlayerDeath_Implementation()
{
	APlayerController* PlayerController = Cast<APlayerController>(Controller);
	if (PlayerController)
	{
		PlayerController->DisableInput(PlayerController);
		PlayerController->UnPossess();
		if (UWorld* World = GetWorld())
		{
			FActorSpawnParameters SpawnParameters;
			AStaticSpectatorPawn* SpecPawn = World->SpawnActor<AStaticSpectatorPawn>(AStaticSpectatorPawn::StaticClass(), GetActorLocation(), FRotator::ZeroRotator, SpawnParameters);
			if (SpecPawn)
			{
				PlayerController->Possess(SpecPawn);
			}
		}
	}
	SetActorRotation(FRotator(-90.f, -15.f, -30.f));
	SetLifeSpan(3.f);
}

bool AShooterPlayerCharacter::PlayerDeath_Validate()
{
	return true;
}



