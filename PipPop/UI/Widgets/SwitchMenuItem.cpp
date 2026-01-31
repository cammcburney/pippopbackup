// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Widgets/SwitchMenuItem.h"
#include "Components/Button.h"
#include "Mode/AppearanceSubsystem.h"

void USwitchMenuItem::NativeConstruct()
{
	LeftButton->OnClicked.AddDynamic(this, &USwitchMenuItem::OnLeftClick);
	RightButton->OnClicked.AddDynamic(this, &USwitchMenuItem::OnRightClick);
}

void USwitchMenuItem::NextItem(const int32 Iteration)
{
	if (Owner && !AppearanceSubsystem)
	{
		AppearanceSubsystem = Owner->GetAppearanceSubsystem();
	}
	if (!AppearanceSection.IsNone() && SkeletalMeshComponent && AppearanceSubsystem)
	{
		const int32 NextIndex = Index + Iteration;
		const int32 MaxIndex = AppearanceSubsystem->GetSectionLength(AppearanceSection, AppearanceType);
		Index = GetNextValidIndex(NextIndex, MaxIndex);
		SelectAssetAction();
	}
}

int32 USwitchMenuItem::GetNextValidIndex(const int32 NextIndex, const int32 MaxIndex)
{
	if (NextIndex < 0)
	{
		return MaxIndex - 1;
	}
	if (NextIndex >= MaxIndex)
	{
		return 0;
	}
	return NextIndex;
}

void USwitchMenuItem::SelectAssetAction()
{
	if (!AppearanceSubsystem) {return;}
	switch (AppearanceType)
	{
	case EAppearanceType::Mesh:
		{
			USkeletalMesh* Mesh = AppearanceSubsystem->LoadAppearanceAsset(AppearanceSection, Index, &FAppearanceInfo::Mesh);
			SkeletalMeshComponent->SetSkeletalMesh(Mesh);
			break;
		}
	case EAppearanceType::Material:
		{
			UMaterialInterface* Material = AppearanceSubsystem->LoadAppearanceAsset(AppearanceSection, Index, &FAppearanceInfo::Material);
			SkeletalMeshComponent->SetMaterial(0, Material);
			break;
		}
	}
}
