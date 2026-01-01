// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/CustomisationManager.h"
#include "Character/CustomisationPawn.h"
#include "Character/Components/AppearanceComponent.h"
#include "Mode/AppearanceSubsystem.h"
#include "Components/VerticalBox.h"
#include "Kismet/GameplayStatics.h"
#include "Mode/PipPopGameInstance.h"
#include "Widgets/SwitchMenuItem.h"

void UCustomisationManager::Init()
{
	if (!GetWorld()) {return;}
	const UPipPopGameInstance* GameInst = Cast<UPipPopGameInstance>(GetWorld()->GetGameInstance());
	if (!GameInst) {return;}
	AppearanceSubsystem = GameInst->GetSubsystem<UAppearanceSubsystem>();
	if (const ACustomisationPawn* PlayerPawn = Cast<ACustomisationPawn>(UGameplayStatics::GetPlayerPawn(this, 0)))
	{
		Appearance = PlayerPawn->GetAppearance();
		if (!Appearance) {return;}
		const TMap<EAppearance, USkeletalMeshComponent*> AppearanceMap = Appearance->GetAppearanceMap();
		if (AppearanceMap.IsEmpty()) {return;}
		for (const TPair AppearancePair : AppearanceMap)
		{
			if (!SwitchMenuItemClass) {return;}
			if (USwitchMenuItem* SwitchMenu = CreateWidget<USwitchMenuItem>(this, SwitchMenuItemClass))
			{
				Panel->AddChild(SwitchMenu);
				SwitchMenu->SetSection(Appearance->GetSectionName(AppearancePair.Key));
				SwitchMenu->SetAppearanceReference(AppearancePair.Value);
				const FName Key = Appearance->GetSectionName(AppearancePair.Key);
				const FName ItemName = AppearanceSubsystem->GetSectionName(Key);
				SwitchMenu->SetItemName(ItemName);
				SwitchMenu->SetManager(this);
			}
		}
	}
}
