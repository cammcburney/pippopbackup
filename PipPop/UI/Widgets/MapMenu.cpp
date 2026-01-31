// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Widgets/MapMenu.h"

#include "Blueprint/WidgetTree.h"
#include "Components/Button.h"
#include "GameFramework/InputDeviceSubsystem.h"

void UMapMenu::NativeConstruct()
{
	Super::NativeConstruct();
	if (LeftButton)
	{
		LeftButton->OnClicked.AddDynamic(this, &UMapMenu::OnLeftClick);
	}
	if (RightButton)
	{
		RightButton->OnClicked.AddDynamic(this, &UMapMenu::OnRightClick);
	}
	TArray<UWidget*> Widgets;
	WidgetTree->GetAllWidgets(Widgets);
	for (UWidget* Widget : Widgets)
	{
		if (UButton* Button = Cast<UButton>(Widget))
		{
			Buttons.Add(Button);
		}
	}
	if (Buttons.Num())
	{
		for (int32 i = 0; i < Buttons.Num(); ++i)
		{
			Buttons[i]->OnPressed.AddDynamic(this, &UMapMenu::SetLevel);
		}
	}
}

void UMapMenu::SetLevel()
{
	for (int32 i = 0; i < Buttons.Num(); ++i)
	{
		if (Buttons[i]->IsPressed())
		{
			const UEnum* LevelEnums = StaticEnum<ELevelStructs>();
			if (LevelEnums->IsValidEnumValue(i))
			{
				LevelToLoad = static_cast<ELevelStructs>(i);
				SetLevelDelegate.ExecuteIfBound();
			}
			break;
		}
	}
}

void UMapMenu::NextItem(const int32 Iteration)
{
	const UEnum* LevelEnums = StaticEnum<ELevelStructs>();
	if (MapIndexMultiplier + Iteration == 0)
	{
		MapIndexMultiplier = 1;
	}
	else if (MapIndexMultiplier <= LevelEnums->NumEnums() / GridSize && LevelEnums->NumEnums() / GridSize > 1)
	{
		MapIndexMultiplier = MapIndexMultiplier + Iteration;
	}
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Index: " + FString::FromInt(MapIndexMultiplier)));
}