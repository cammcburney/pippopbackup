// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Actors/ItemRotationSwitcher.h"

#include "AssetTypeActions/AssetDefinition_SoundBase.h"
#include "Character/Components/AppearanceComponent.h"
#include "Character/CustomisationPawn.h"
#include "Components/AudioComponent.h"

AItemRotationSwitcher::AItemRotationSwitcher()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AItemRotationSwitcher::Tick(float DeltaSeconds)
{
	if (!bRotate) {return;}
	Super::Tick(DeltaSeconds);
	if (APlayerController* PlayerController = GetWorld()->GetFirstPlayerController())
	{
		if (!PlayerController->IsInputKeyDown(EKeys::LeftMouseButton))
		{
			bRotate = false;
			if (AudioComponent)
			{
				AudioComponent->Stop();
				AudioComponent = nullptr;
			}
			return;
		}
		if (ACustomisationPawn* CustomisationPawn = Cast<ACustomisationPawn>(PlayerController->GetPawn()))
		{
			if (UAppearanceComponent* AppearanceComponent = CustomisationPawn->GetAppearance())
			{
				TMap<EAppearance, USkeletalMeshComponent*> AppearanceComponents =AppearanceComponent->GetAppearanceMap();
				for (const auto& AppearanceElem : AppearanceComponents)
				{
					if (USkeletalMeshComponent* AppearanceItem = AppearanceElem.Value)
					{
						if (AppearanceItem)
						{
							FRotator Rotation = AppearanceElem.Value->GetComponentRotation();
							FRotator RotationVector = FRotator::ZeroRotator;
							switch (RotationDirection)
							{
								case EDirections::RIGHT:
									{
										RotationVector = FRotator(0, -RotationRate, 0);
										break;
									}
								case EDirections::LEFT:
									{
										RotationVector = FRotator(0, RotationRate, 0);
										break;
									}
								case EDirections::UP:
									{
										RotationVector = FRotator(0, 0, RotationRate);
										break;
									}
								case EDirections::DOWN:
									{
										RotationVector = FRotator(0, 0, -RotationRate);
										break;
									}
							}
							AppearanceElem.Value->SetWorldRotation(Rotation + RotationVector);
						}
					}
				}
			}
		}
	}
}

void AItemRotationSwitcher::Interact_Implementation(UPrimitiveComponent* InteractedComponent)
{
	bRotate = true;
	AudioComponent = PlaySoundEffect(InteractSoundEffect);
}
