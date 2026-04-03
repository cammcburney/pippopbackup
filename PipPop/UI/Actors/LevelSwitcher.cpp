// Fill out your copyright notice in the Description page of Project Settings.


#include "LevelSwitcher.h"
#include "Mode/PipPopGameInstance.h"
#include "Mode/Subsystem/LevelSubsystem.h"

ALevelSwitcher::ALevelSwitcher()
{
	bInteracted = false;
}

void ALevelSwitcher::Interact_Implementation(UPrimitiveComponent* InteractedComponent)
{
	ASwitcherBase::Interact_Implementation(InteractedComponent);
	if (!InteractedComponent || bInteracted) {return;}
	bInteracted = true;
	UPipPopGameInstance* GameInst = Cast<UPipPopGameInstance>(GetWorld()->GetGameInstance());
	if (!GameInst) {bInteracted = false; return;}
	ULevelSubsystem* Subsystem = GameInst->GetSubsystem<ULevelSubsystem>();
	if (!Subsystem) {bInteracted = false; return;}
	const UEnum* LevelEnums = StaticEnum<ELevelStructs>();
	if (!LevelEnums) {bInteracted = false; return;}
	const FName LevelIdentifier = FName(FString::FromInt(static_cast<int8>(LevelReference)));
	const FName LevelName = Subsystem->GetLevelName(LevelIdentifier);
	if (LevelName.IsNone()) {bInteracted = false; return;}
	GameInst->LoadLevel(LevelName);
}
