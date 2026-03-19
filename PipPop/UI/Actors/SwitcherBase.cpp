// Fill out your copyright notice in the Description page of Project Settings.


#include "SwitcherBase.h"

ASwitcherBase::ASwitcherBase()
{
	PrimaryActorTick.bCanEverTick = false;
	
	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComponent"));
	check(StaticMeshComponent);
	StaticMeshComponent->SetupAttachment(RootComponent);
	StaticMeshComponent->SetRenderCustomDepth(true);
	StaticMeshComponent->SetCustomDepthStencilValue(1);
	
	TextComponent = CreateDefaultSubobject<UText3DComponent>(TEXT("TextComponent"));
	check(TextComponent);
	TextComponent->SetupAttachment(RootComponent);
}