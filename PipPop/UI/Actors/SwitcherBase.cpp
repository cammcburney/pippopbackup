// Fill out your copyright notice in the Description page of Project Settings.


#include "SwitcherBase.h"

ASwitcherBase::ASwitcherBase()
{
	PrimaryActorTick.bCanEverTick = false;
	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComponent"));
	TextComponent = CreateDefaultSubobject<UText3DComponent>(TEXT("TextComponent"));
	if (StaticMeshComponent)
	{
		RootComponent = StaticMeshComponent;
		LoadMesh();
	}
	if (TextComponent)
	{
		TextComponent->SetupAttachment(RootComponent);
	}
}

void ASwitcherBase::BeginPlay()
{
	Super::BeginPlay();
	LoadMesh();
}

void ASwitcherBase::Interact_Implementation(UPrimitiveComponent* InteractedComponent)
{
}

void ASwitcherBase::LoadMesh() const
{
	if (StaticMeshComponent)
	{
		StaticMeshComponent->SetRenderCustomDepth(true);
		StaticMeshComponent->SetCustomDepthStencilValue(1);
		if (StaticMesh)
		{
			StaticMeshComponent->SetStaticMesh(StaticMesh);
			if (MeshMaterial)
			{
				StaticMeshComponent->SetMaterial(0, MeshMaterial);
			}
		}
	}
}

