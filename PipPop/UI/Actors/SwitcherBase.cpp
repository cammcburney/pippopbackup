// Fill out your copyright notice in the Description page of Project Settings.


#include "SwitcherBase.h"
#include "Kismet/GameplayStatics.h"

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

void ASwitcherBase::Interact_Implementation(UPrimitiveComponent* InteractedComponent)
{
	PlaySoundEffect(InteractSoundEffect);
}

UAudioComponent* ASwitcherBase::PlaySoundEffect(const FSoundEffect PlayedSoundEffect)
{
	if (!PlayedSoundEffect.Sound) {return nullptr;}
	return UGameplayStatics::SpawnSoundAtLocation(this,PlayedSoundEffect.Sound, GetActorLocation(), FRotator::ZeroRotator, PlayedSoundEffect.Volume, PlayedSoundEffect.Pitch, PlayedSoundEffect.StartTime);
}