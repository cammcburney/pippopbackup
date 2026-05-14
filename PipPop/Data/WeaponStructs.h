#pragma once

#include "CoreMinimal.h"
#include "LevelStructs.generated.h"
#include "Character/Weapons/BaseWeapon.h"

USTRUCT(BlueprintType)
struct PIPPOP_API FLevelNames: public FTableRowBase
{
	GENERATED_USTRUCT_BODY()
	
	UPROPERTY()
	TObjectPtr<ABaseWeapon> PrimaryWeapon;

	UPROPERTY()
	TObjectPtr<ABaseWeapon> SecondaryWeapon;

	UPROPERTY()
	TObjectPtr<ABaseWeapon> SpecialWeapon;

	UPROPERTY()
	TObjectPtr<ABaseWeapon> GrenadeWeapon;
};

UENUM()
enum class EWeaponTypes : uint8
{
	CORE = 0 UMETA(DisplayName = "Core"),
	SPECIAL = 1 UMETA(DisplayName = "Special"),
	GRENADE = 2 UMETA(DisplayName = "Grenade")
};