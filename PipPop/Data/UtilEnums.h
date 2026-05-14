#pragma once

#include "CoreMinimal.h"
#include "UtilEnums.generated.h"

UENUM()
enum class EDirections : uint8
{
	UP = 0 UMETA(DisplayName = "UP"),
	DOWN = 1 UMETA(DisplayName = "DOWN"),
	LEFT = 2 UMETA(DisplayName = "LEFT"),
	RIGHT = 3 UMETA(DisplayName = "RIGHT"),
};