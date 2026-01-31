// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Actors/OnlineSessionSwitcher.h"
#include "Components/WidgetComponent.h"
#include "Mode/PipPopGameInstance.h"
#include "UI/Widgets/MapMenu.h"
#include "MediaPlayer.h"
#include "MediaSource.h"

AOnlineSessionSwitcher::AOnlineSessionSwitcher()
{
	MapMenuComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("MapMenu"));
	MediaPlayer = CreateDefaultSubobject<UMediaPlayer>(TEXT("MediaPlayer"));
	MapMenuComponent->SetComponentTickEnabled(true);
	PrimaryActorTick.bCanEverTick = true;
	MapMenuComponent->SetupAttachment(RootComponent);
	MapMenuComponent->SetDrawSize(FVector2D(1028.0f, 248.0f));
	MapMenuComponent->SetWidgetSpace(EWidgetSpace::World);
	MapMenuComponent->SetRelativeRotation(FRotator(0, -180.f, 0));
}

void AOnlineSessionSwitcher::BeginPlay()
{
	Super::BeginPlay();
	if (MapMenuComponent && WidgetClass)
	{
		MapMenuComponent->SetWidgetClass(WidgetClass);
		if (UMapMenu* Menu = Cast<UMapMenu>(MapMenuComponent->GetWidget()))
		{
			Menu->SetLevelDelegate.BindUObject(this, &AOnlineSessionSwitcher::SetLevelReference);
		}
	}
	if (MediaPlayer && MapVideo)
	{
		MediaPlayer->OpenSource(MapVideo);
		MediaPlayer->SetLooping(true);
	}
}

void AOnlineSessionSwitcher::Interact_Implementation(UPrimitiveComponent* InteractedComponent)
{
	if (!InteractedComponent) {return;}
	if (InteractedComponent != MapMenuComponent)
	{
		UPipPopGameInstance* GameInst = Cast<UPipPopGameInstance>(GetWorld()->GetGameInstance());
		if (!GameInst) {return;}
		GameInst->HostSession(FName(TEXT("MySession")));
		Super::Interact_Implementation(InteractedComponent);
	}
}
	
void AOnlineSessionSwitcher::SetLevelReference()
{
	if (const UMapMenu* MapMenu = Cast<UMapMenu>(MapMenuComponent->GetWidget()))
	{
		LevelReference = MapMenu->GetLevel();
	}
}
