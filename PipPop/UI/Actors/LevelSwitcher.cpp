// Fill out your copyright notice in the Description page of Project Settings.


#include "LevelSwitcher.h"
#include "Mode/PipPopGameInstance.h"
#include "Mode/Subsystem/LevelSubsystem.h"

void ALevelSwitcher::Interact_Implementation(UPrimitiveComponent* InteractedComponent)
{
	if (!InteractedComponent) {return;}
	if (UWorld* World = GetWorld())
	{
		UPipPopGameInstance* GameInst = Cast<UPipPopGameInstance>(World->GetGameInstance());
		if (!GameInst) {return;}
		ULevelSubsystem* Subsystem = GameInst->GetSubsystem<ULevelSubsystem>();
		if (!Subsystem) {return;}
		const UEnum* LevelEnums = StaticEnum<ELevelStructs>();
		if (!LevelEnums) {return;}
		const FName LevelIdentifier = FName(FString::FromInt(static_cast<int8>(LevelReference)));
		const FName LevelName = Subsystem->GetLevelName(LevelIdentifier);
		if (LevelName.IsNone()) {return;}
		GameInst->LoadLevel(LevelName);
	}
}
