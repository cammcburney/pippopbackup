// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/SwitchMenuItem.h"
#include "Components/Button.h"
#include "Mode/AppearanceSubsystem.h"

void USwitchMenuItem::NativeConstruct()
{
	LeftButton->OnClicked.AddDynamic(this, &USwitchMenuItem::OnLeftClick);
	RightButton->OnClicked.AddDynamic(this, &USwitchMenuItem::OnRightClick);
}

void USwitchMenuItem::NextItem(const int32 Iteration)
{
	if (Owner && !AppearanceSection.IsNone() && SkeletalMeshComponent)
	{
		UAppearanceSubsystem* Subsystem = Owner->GetAppearanceSubsystem();
		int32 NextIndex = Index + Iteration;
		const int32 MaxIndex = Subsystem->GetSectionLength(AppearanceSection);
		if (NextIndex < 0)
		{
			NextIndex = MaxIndex - 1;
		}
		else if (NextIndex >= MaxIndex - 1)
		{
			NextIndex = 0;
		}
		Index = NextIndex;
		USkeletalMesh* Item = Subsystem->LoadAppearanceAsset(AppearanceSection, Index, &FAppearanceInfo::Mesh);
		SkeletalMeshComponent->SetSkeletalMesh(Item);
	}
}



