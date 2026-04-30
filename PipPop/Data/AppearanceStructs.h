#pragma once

#include "CoreMinimal.h"
#include "AppearanceStructs.generated.h"

UENUM(BlueprintType)
enum class EAppearance : uint8
{
	HEAD UMETA(DisplayName = "HEAD"),
	EYES UMETA(DisplayName = "EYES"),
	MOUTH UMETA(DisplayName = "MOUTH"),
	EARS UMETA(DisplayName = "EARS"),
	TORSO UMETA(DisplayName = "TORSO"),
	LEGS UMETA(DisplayName = "LEGS"),
	TAIL UMETA(DisplayName = "TAIL"),
	HAT UMETA(DisplayName = "HAT"),
	ACCESSORY_1 UMETA(DisplayName = "ACCESSORY_1"),
	ACCESSORY_2 UMETA(DisplayName = "ACCESSORY_2")
};

// Essential in iteration loops, without it compile will fail
ENUM_RANGE_BY_FIRST_AND_LAST(EAppearance, EAppearance::HEAD, EAppearance::ACCESSORY_2);

namespace AppearanceUtils
{
	static FName GetSectionName(const EAppearance AppearanceEnum)
	{
		return FName(UEnum::GetDisplayValueAsText(AppearanceEnum).ToString());
	}
}

USTRUCT(BlueprintType)
struct PIPPOP_API FAppearanceInfo: public FTableRowBase
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TArray<TObjectPtr<USkeletalMesh>> Mesh;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TArray<TObjectPtr<UMaterialInterface>> Material;
};