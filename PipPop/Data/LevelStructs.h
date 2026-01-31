#pragma once

#include "CoreMinimal.h"
#include "LevelStructs.generated.h"

USTRUCT(BlueprintType)
struct PIPPOP_API FLevelNames: public FTableRowBase
{
	GENERATED_USTRUCT_BODY()
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FName LevelName;
};

UENUM()
enum class ELevelStructs : uint8
{
	Main_Menu = 0 UMETA(DisplayName = "Main Menu"),
	Customisation = 1 UMETA(DisplayName = "Customisation")
};