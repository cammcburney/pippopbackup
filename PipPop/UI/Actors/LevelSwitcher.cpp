// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Actors/LevelSwitcher.h"
#include "Kismet/GameplayStatics.h"

ALevelSwitcher::ALevelSwitcher()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, "Construct");
	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComponent"));
	RootComponent = StaticMeshComponent;
	StaticMeshComponent->SetRenderCustomDepth(true);
	StaticMeshComponent->SetCustomDepthStencilValue(1);
	if (StaticMeshComponent && StaticMesh)
	{
		StaticMeshComponent->SetStaticMesh(StaticMesh);
		StaticMeshComponent->SetMaterial(0, MeshMaterial);
	}
}

void ALevelSwitcher::BeginPlay()
{
	Super::BeginPlay();
	if (StaticMeshComponent && StaticMesh)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, "LevelSwitcherBP");
		StaticMeshComponent->SetStaticMesh(StaticMesh);
		StaticMeshComponent->SetMaterial(0, MeshMaterial);
		StaticMeshComponent->SetRenderCustomDepth(true);
		StaticMeshComponent->SetCustomDepthStencilValue(1);
	}
}

void ALevelSwitcher::Interact_Implementation(UPrimitiveComponent* InteractedComponent)
{
	if (!InteractedComponent) {return;}
	UGameplayStatics::OpenLevel(this, TEXT("/Game/Levels/LVL_Customise"), TRAVEL_Absolute);
}

