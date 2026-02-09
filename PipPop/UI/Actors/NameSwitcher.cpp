#include "NameSwitcher.h"
#include "Components/ChildActorComponent.h"
#include "UI/Widgets/CustomText.h"
#include "Text3DComponent.h"
#include "Controller/CustomisationController.h"
#include "Kismet/GameplayStatics.h"
#include "Mode/PipPopGameInstance.h"

ANameSwitcher::ANameSwitcher()
{
	TextComponent = CreateDefaultSubobject<UText3DComponent>(TEXT("PlayerName"));
	TextComponent->SetupAttachment(RootComponent);
}

void ANameSwitcher::BeginPlay()
{
	Super::BeginPlay();
	if (UPipPopGameInstance* GameInstance = Cast<UPipPopGameInstance>(GetGameInstance()))
	{
		const FString NewPlayerName = GameInstance->GetPlayerName().ToString();
		GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Yellow, FString::Printf(TEXT("%s"), *NewPlayerName));
		if (TextComponent)
		{
			TextComponent->SetText(FText::FromString(NewPlayerName));
		}
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Yellow, TEXT("No Name"));
	}
}

void ANameSwitcher::Interact_Implementation(UPrimitiveComponent* InteractedComponent)
{
	GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Yellow, TEXT("Interact_Implementation"));
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
				EditableText->OnTextChanged.AddDynamic(this, &ANameSwitcher::PlayerNameTextUpdated);
				EditableText->OnTextCommitted.AddDynamic(this, &ANameSwitcher::PlayerNameTextCommitted);
				EditableText->SetText(TextComponent->GetText());
			}
		}
	}
}

void ANameSwitcher::PlayerNameTextUpdated(const FText& NewText)
{
	if (NewText.ToString().Len() > MaxStringLength)
	{
		const FString CurrentText = CustomTextWidget->EditableTextBox->GetText().ToString();
		CustomTextWidget->EditableTextBox->SetText(FText::FromString(CurrentText.Left(MaxStringLength)));
		return;
	}
	if (TextComponent)
	{
		TextComponent->SetText(NewText);
	}
}

void ANameSwitcher::PlayerNameTextCommitted(const FText& Text, ETextCommit::Type CommitMethod)
{
	if (CommitMethod == ETextCommit::OnEnter)
	{
		PlayerNameTextUpdated(Text);
		if (UPipPopGameInstance* GameInstance = Cast<UPipPopGameInstance>(GetGameInstance()))
		{
			GameInstance->SetPlayerName(FName(Text.ToString()));
		}
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