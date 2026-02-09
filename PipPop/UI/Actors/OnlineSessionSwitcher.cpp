// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Actors/OnlineSessionSwitcher.h"
#include "Components/WidgetComponent.h"
#include "Components/ChildActorComponent.h"
#include "Mode/PipPopGameInstance.h"
#include "UI/Widgets/CustomText.h"
#include "UI/Widgets/MapMenu.h"
#include "Text3DComponent.h"
#include "MediaPlayer.h"
#include "MediaSource.h"
#include "Controller/CustomisationController.h"
#include "Kismet/GameplayStatics.h"

AOnlineSessionSwitcher::AOnlineSessionSwitcher()
{
	MapMenuComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("MapMenu"));
	MediaPlayer = CreateDefaultSubobject<UMediaPlayer>(TEXT("MediaPlayer"));
	SessionName = CreateDefaultSubobject<UText3DComponent>(TEXT("SessionName"));
	SessionName->SetupAttachment(RootComponent);
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
	if (InteractedComponent->GetAttachmentRoot()->GetName().Contains(TEXT("SessionName")))
	{
		if (ACustomisationController* Controller = Cast<ACustomisationController>(UGameplayStatics::GetPlayerController(this, 0)))
		{
			if (!HiddenTextWidgetClass) {return;}
			CustomTextWidget = CreateWidget<UCustomText>(Controller, HiddenTextWidgetClass);
			if (CustomTextWidget)
			{
				Controller->SetInputMode(FInputModeUIOnly());
				Controller->EnableInput(Controller);
				CustomTextWidget->AddToViewport();
				if (UEditableTextBox* EditableText = CustomTextWidget->EditableTextBox)
				{
					EditableText->SetFocus();
					EditableText->SetKeyboardFocus();
					EditableText->OnTextChanged.AddDynamic(this, &AOnlineSessionSwitcher::SessionNameTextUpdated);
					EditableText->OnTextCommitted.AddDynamic(this, &AOnlineSessionSwitcher::SessionNameTextCommitted);
					EditableText->SetText(SessionName->GetText());
				}
			}
		}
	}
	if (InteractedComponent != MapMenuComponent && !InteractedComponent->GetAttachmentRoot()->GetName().Contains(TEXT("SessionName")))
	{
		UPipPopGameInstance* GameInst = Cast<UPipPopGameInstance>(GetWorld()->GetGameInstance());
		if (!GameInst) {return;}
		GameInst->HostSession(*SessionName->GetText().ToString());
		Super::Interact_Implementation(InteractedComponent);
	}
}

void AOnlineSessionSwitcher::SessionNameTextUpdated(const FText& NewText)
{
	if (NewText.ToString().Len() > 12)
	{
		const FString CurrentText = CustomTextWidget->EditableTextBox->GetText().ToString();
		CustomTextWidget->EditableTextBox->SetText(FText::FromString(CurrentText.Left(12)));
		return;
	}
	if (SessionName)
	{
		SessionName->SetText(NewText);
	}
}

void AOnlineSessionSwitcher::SessionNameTextCommitted(const FText& Text, ETextCommit::Type CommitMethod)
{
	if (CommitMethod == ETextCommit::OnEnter)
	{
		SessionNameTextUpdated(Text);
	   if (CustomTextWidget)
	   {
	   		CustomTextWidget->RemoveFromParent();
	   		CustomTextWidget = nullptr;
	   }
		if (ACustomisationController* Controller = Cast<ACustomisationController>(UGameplayStatics::GetPlayerController(this, 0)))
		{
			Controller->SetInputMode(FInputModeGameAndUI());
			Controller->DisableInput(Controller);
		}
	}
}

void AOnlineSessionSwitcher::SetLevelReference()
{
	if (const UMapMenu* MapMenu = Cast<UMapMenu>(MapMenuComponent->GetWidget()))
	{
		LevelReference = MapMenu->GetLevel();
	}
}
