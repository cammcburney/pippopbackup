// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Widgets/CustomisationManager.h"
#include "Character/CustomisationPawn.h"
#include "Character/Components/AppearanceComponent.h"
#include "Mode/AppearanceSubsystem.h"
#include "Components/VerticalBox.h"
#include "Kismet/GameplayStatics.h"
#include "Mode/PipPopGameInstance.h"
#include "UI/Widgets/SwitchMenuItem.h"

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
			if (USwitchMenuItem* MeshMenu = CreateWidget<USwitchMenuItem>(this, SwitchMenuItemClass))
			{
				MeshPanel->AddChild(MeshMenu);
				MeshMenu->SetType(EAppearanceType::Mesh);
				MeshMenu->SetSection(Appearance->GetSectionName(AppearancePair.Key));
				MeshMenu->SetAppearanceReference(AppearancePair.Value);
				const FName Key = Appearance->GetSectionName(AppearancePair.Key);
				const FName ItemName = AppearanceSubsystem->GetSectionName(Key);
				MeshMenu->SetItemName(ItemName);
				MeshMenu->SetManager(this);
				MeshMenus.Add(MeshMenu);
			}
			if (USwitchMenuItem* MaterialMenu = CreateWidget<USwitchMenuItem>(this, SwitchMenuItemClass))
			{
				MaterialPanel->AddChild(MaterialMenu);
				MaterialMenu->SetType(EAppearanceType::Material);
				MaterialMenu->SetSection(Appearance->GetSectionName(AppearancePair.Key));
				MaterialMenu->SetAppearanceReference(AppearancePair.Value);
				const FName Key = Appearance->GetSectionName(AppearancePair.Key);
				const FName ItemName = AppearanceSubsystem->GetSectionName(Key);
				MaterialMenu->SetItemName(ItemName);
				MaterialMenu->SetManager(this);
				MaterialMenus.Add(MaterialMenu);
			}
		}
	}
}

TArray<int32> UCustomisationManager::GetMeshIndexes()
{
	TArray<int32> Indexes;
	for (int32 i = 0; i < MeshMenus.Num(); i++)
	{
		if (MeshMenus.IsValidIndex(i))
		{
			Indexes.Add(MeshMenus[i]->GetIndex());
		}
	}
	return Indexes;
}