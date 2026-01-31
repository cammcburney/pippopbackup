// Fill out your copyright notice in the Description page of Project Settings.


#include "RotationSwitcher.h"
#include "Character/MenuPawn.h"
#include "Components/TimelineComponent.h"
#include "Kismet/GameplayStatics.h"

ARotationSwitcher::ARotationSwitcher()
{
	Timeline = CreateDefaultSubobject<UTimelineComponent>("Timeline");
}

void ARotationSwitcher::BeginPlay()
{
	Super::BeginPlay();
	if (Timeline)
	{
		FOnTimelineFloat TimelineProgress;
		TimelineProgress.BindUFunction(this, FName("ProgressRotation"));
		FOnTimelineEvent TimelineComplete;
		TimelineComplete.BindUFunction(this, FName("RotationCompleted"));
		
		if (UCurveFloat* Curve = NewObject<UCurveFloat>(this, "Curve"))
		{
			Curve->FloatCurve.UpdateOrAddKey(0.f, 0.f);
			Curve->FloatCurve.UpdateOrAddKey(1.f, 90.f);
			Timeline->AddInterpFloat(Curve, TimelineProgress, FName("TimelineProgress"));
		}
		Timeline->SetTimelineLength(4.f);
		Timeline->SetTimelineFinishedFunc(TimelineComplete);
	}
}

void ARotationSwitcher::Interact_Implementation(UPrimitiveComponent* InteractedComponent)
{
	if (Timeline && !Timeline->IsPlaying())
	{
		if (AMenuPawn* Player = Cast<AMenuPawn>(UGameplayStatics::GetPlayerPawn(this, 0)))
		{
			CameraRotation = Player->GetCameraRotation();
			if (APlayerController* PlayerController = Cast<APlayerController>(UGameplayStatics::GetPlayerController(this, 0)))
			{
				Player->DisableInput(PlayerController);
			}
		}
		Timeline->PlayFromStart();
	}
}

void ARotationSwitcher::ProgressRotation(float Value) const
{
	if (const AMenuPawn* Player = Cast<AMenuPawn>(UGameplayStatics::GetPlayerPawn(this, 0)))
	{
		float RotationValue = Value;
		if (InvertRotation)
		{
			RotationValue = -Value;
		}
		const FRotator NewRotation = FRotator(CameraRotation.Pitch, CameraRotation.Yaw + RotationValue, CameraRotation.Roll);
		Player->SetCameraRotation(NewRotation);
	}
}

void ARotationSwitcher::RotationCompleted() const
{
	if (AMenuPawn* Player = Cast<AMenuPawn>(UGameplayStatics::GetPlayerPawn(this, 0)))
	{
		if (APlayerController* PlayerController = Cast<APlayerController>(UGameplayStatics::GetPlayerController(this, 0)))
		{
			Player->EnableInput(PlayerController);
		}
	}
}
