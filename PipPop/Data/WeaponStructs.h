#pragma once

#include "CoreMinimal.h"
#include "Character/Weapons/BaseWeapon.h"
#include "WeaponStructs.generated.h"

USTRUCT(BlueprintType)
struct PIPPOP_API FEquippedWeapons
{
	GENERATED_USTRUCT_BODY()
	
	UPROPERTY()
	TSubclassOf<ABaseWeapon> PrimaryWeapon = nullptr;

	UPROPERTY()
	TSubclassOf<ABaseWeapon> SecondaryWeapon = nullptr;

	UPROPERTY()
	TSubclassOf<ABaseWeapon> SpecialWeapon = nullptr;

	UPROPERTY()
	TSubclassOf<ABaseWeapon> TacticalWeapon = nullptr;
};

UENUM()
enum class EWeaponTypes : uint8
{
	PRIMARY = 0 UMETA(DisplayName = "Primary"),
	SECONDARY = 1 UMETA(DisplayName = "Secondary"),
	SPECIAL = 2 UMETA(DisplayName = "Special"),
	TACTICAL = 3 UMETA(DisplayName = "Tactical")
};