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
	GEngine->AddOnScreenDebugMessage(-1, 1.5, FColor::Red, FString::Printf(TEXT("CLICK")));
	GEngine->AddOnScreenDebugMessage(-1, 1.5, FColor::Red, FString::Printf(TEXT("Owner, %s"), Owner ? TEXT("true") : TEXT("false")));
	GEngine->AddOnScreenDebugMessage(-1, 1.5, FColor::Red, FString::Printf(TEXT("AppearanceSection, %s"), !AppearanceSection.IsNone() ? TEXT("true") : TEXT("false")));
	GEngine->AddOnScreenDebugMessage(-1, 1.5, FColor::Red, FString::Printf(TEXT("SkeletalMeshComponent, %s"), SkeletalMeshComponent ? TEXT("true") : TEXT("false")));
	
	if (Owner && !AppearanceSection.IsNone() && SkeletalMeshComponent)
	{
		GEngine->AddOnScreenDebugMessage(-1, 1.5, FColor::Red, FString::Printf(TEXT("%d"), Iteration));
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
		USkeletalMesh* Item = Subsystem->LoadAppearanceItem(AppearanceSection, Index);
		SkeletalMeshComponent->SetSkeletalMesh(Item);
		
	}
}



