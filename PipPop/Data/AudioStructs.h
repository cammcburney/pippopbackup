#pragma once

#include "CoreMinimal.h"
#include "AudioStructs.generated.h"

USTRUCT(BlueprintType)
struct PIPPOP_API FSoundEffect
{
	GENERATED_USTRUCT_BODY()
	
	UPROPERTY(EditAnywhere, Category="Audio")
	TObjectPtr<USoundBase> Sound = nullptr;

	UPROPERTY(EditAnywhere, Category="Audio")
	float Volume = 1.f;
	
	UPROPERTY(EditAnywhere, Category="Audio")
	float Pitch = 1.f;

	UPROPERTY(EditAnywhere, Category="Audio")
	float StartTime = 0.f;
};
