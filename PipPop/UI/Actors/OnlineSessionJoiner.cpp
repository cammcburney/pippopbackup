// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Actors/OnlineSessionJoiner.h"
#include "Components/WidgetComponent.h"
#include "UI/Widgets/SessionsDisplay.h"
#include "Mode/Menu/MenuMode.h"
#include "Mode/PipPopGameInstance.h"

// Sets default values
AOnlineSessionJoiner::AOnlineSessionJoiner()
{
	SessionsDisplayComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("SessionsDisplayComponent"));
	SessionsDisplayComponent->SetComponentTickEnabled(true);
	PrimaryActorTick.bCanEverTick = true;
	SessionsDisplayComponent->SetupAttachment(RootComponent);
	SessionsDisplayComponent->SetDrawSize(FVector2D(1028.0f, 248.0f));
	SessionsDisplayComponent->SetWidgetSpace(EWidgetSpace::World);
	FindSessionsCompleteDelegate = FOnFindSessionsCompleteDelegate::CreateUObject(this, &AOnlineSessionJoiner::FindSessionsComplete);
}

// Called when the game starts or when spawned
void AOnlineSessionJoiner::BeginPlay()
{
	Super::BeginPlay();
	if (UWorld* World = GetWorld())
	{
		if (AMenuMode* Menu = Cast<AMenuMode>(World->GetAuthGameMode()))
		{
			Menu->CreateSessionWidgetDelegate.BindUObject(this, &AOnlineSessionJoiner::AddMenuItem);
		}
	}
	if (!SessionsDisplayComponent)
	{
		GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Red, "SessionsDisplayComponent Missing");
	}
	if (!SessionsDisplayClass)
	{
		GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Red, "SessionsDisplayClass Missing");
	}
	if (SessionsDisplayComponent && SessionsDisplayClass)
	{
		SessionsDisplayComponent->SetWidgetClass(SessionsDisplayClass);
	}
}

// Called every frame
void AOnlineSessionJoiner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AOnlineSessionJoiner::Interact_Implementation(UPrimitiveComponent* InteractedComponent)
{
	if (InteractedComponent != SessionsDisplayComponent)
	{
		if (bIsFindingSessions) {return;}
		if (UPipPopGameInstance* GameInstance = Cast<UPipPopGameInstance>(GetWorld()->GetGameInstance()))
		{
			GameInstance->FindSessions();
			bIsFindingSessions = true;
			GameInstance->FindSessionsCompleteDelegate.BindLambda([this](bool bSuccess)
			{
				FindSessionsComplete(bSuccess);
			});
		}
	}
}

void AOnlineSessionJoiner::AddMenuItem(USessionEntry* SessionEntry)
{
	if (const USessionsDisplay* SessionsDisplay = Cast<USessionsDisplay>(SessionsDisplayComponent->GetWidget()))
	{
		SessionsDisplay->AddSession(SessionEntry);
	}
}

void AOnlineSessionJoiner::FindSessionsComplete(bool bSuccess)
{
	SetIsFindingSessions(false);
	GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Green, "Clear Sessions");
	if (bSuccess)
	{
		GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Yellow, "SessionsDisplay Complete");
		if (USessionsDisplay* Display = Cast<USessionsDisplay>(SessionsDisplayComponent->GetWidget()))
		{
			GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Red, "SessionsDisplay Clearing");
			Display->SessionList->ClearChildren();
		}
	}
}