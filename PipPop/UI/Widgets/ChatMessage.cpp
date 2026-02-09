// Fill out your copyright notice in the Description page of Project Settings.


#include "ChatMessage.h"
#include "Kismet/GameplayStatics.h"

void UChatMessage::SetChatMessage(const FText& NewMessage)
{
	NewChatMessage = NewMessage.ToString();
	if (const UWorld* World = GetWorld())
	{
		World->GetTimerManager().SetTimer(
			ChatTimerHandle,
			this,
			&UChatMessage::AddNextCharacter,
			ChatDelay,
			true
		);
	}
}

void UChatMessage::AddNextCharacter()
{
	if (const UWorld* World = GetWorld())
	{
		if (!ChatMessage || CharacterIndex >= NewChatMessage.Len())
		{
			World->GetTimerManager().ClearTimer(ChatTimerHandle);
			return;
		}
		FString CurrentMessage = NewChatMessage.Left(CharacterIndex + 1);
		ChatMessage->SetText(FText::FromString(CurrentMessage));
		if (TextSound)
		{
			const TCHAR LastCharacter = NewChatMessage[CharacterIndex];
			if (LastCharacter != ' ')
			{
				if (APlayerController* PlayerController = GetOwningPlayer())
				{
					if (APawn* Pawn = PlayerController->GetPawn())
					{
						USoundAttenuation* SoundAttenuation = NewObject<USoundAttenuation>(this);
						SoundAttenuation->Attenuation.bSpatialize = true;
						SoundAttenuation->Attenuation.bAttenuate = true;
						SoundAttenuation->Attenuation.AttenuationShape = EAttenuationShape::Sphere;
						// SoundAttenuation->Attenuation.AttenuationShapeExtents. = FDoubleInterval(100, 100, 100);
						SoundAttenuation->Attenuation.FalloffDistance = 300.f;
						SoundAttenuation->Attenuation.dBAttenuationAtMax = -80.f;
						SoundAttenuation->Attenuation.DistanceAlgorithm = EAttenuationDistanceModel::Logarithmic; 
						
						
						
						const float VolumeMultiplier = FMath::RandRange(.8f, 1.2f);
						const float PitchMultiplier = FMath::RandRange(-1.f, 2.f);
						const float StartTime = FMath::RandRange(0.f, .2f);
						UGameplayStatics::SpawnSoundAttached(
							TextSound,
							SpeakingPawn->GetRootComponent(),
							NAME_None,
							FVector::ZeroVector,
							EAttachLocation::KeepRelativeOffset,
							true,
							VolumeMultiplier,
							PitchMultiplier,
							StartTime,
							SoundAttenuation);
					}
				}
			}
		}
		CharacterIndex++;
	}
}
