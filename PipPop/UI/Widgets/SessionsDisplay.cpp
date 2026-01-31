// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Widgets/SessionsDisplay.h"

void USessionsDisplay::NativeConstruct()
{
	Super::NativeConstruct();
}

void USessionsDisplay::AddSession(USessionEntry* NewSession) const
{
	if (SessionList)
	{
		SessionList->AddChild(NewSession);
	}
}
