// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Widgets/CustomText.h"

#include "Blueprint/WidgetTree.h"

void UCustomText::NativePreConstruct()
{
	Super::NativePreConstruct();
	SetIsFocusable(true);
}
