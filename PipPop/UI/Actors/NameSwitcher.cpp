#include "NameSwitcher.h"
#include "Components/ChildActorComponent.h"
#include "UI/Widgets/CustomText.h"
#include "Components/TextBlock.h"
#include "Text3DComponent.h"
#include "Controller/CustomisationController.h"
#include "Kismet/GameplayStatics.h"
#include "Mode/PipPopGameInstance.h"

ANameSwitcher::ANameSwitcher()
{
	Caret = CreateDefaultSubobject<UText3DComponent>(TEXT("Caret"));
	check(Caret);
	Caret->SetupAttachment(RootComponent);
	Caret->SetVisibility(false);
}

void ANameSwitcher::ShowCaret()
{
	bCaretVisible = true;
	Caret->SetVisibility(true);
	GetWorld()->GetTimerManager().SetTimer(CaretTimerHandle, this, &ANameSwitcher::ToggleCaret, ToggleTime, true);
}

void ANameSwitcher::ToggleCaret()
{
	bCaretVisible = !bCaretVisible;
	Caret->SetVisibility(bCaretVisible);
}

void ANameSwitcher::HideCaret()
{
	GetWorld()->GetTimerManager().ClearTimer(CaretTimerHandle);
	Caret->SetVisibility(false);
	bCaretVisible = false;
}

void ANameSwitcher::BeginPlay()
{
	Super::BeginPlay();
	if (UPipPopGameInstance* GameInstance = Cast<UPipPopGameInstance>(GetGameInstance()))
	{
		const FString NewPlayerName = GameInstance->GetPlayerName().ToString();
		if (TextComponent)
		{
			TextComponent->SetText(FText::FromString(NewPlayerName));
		}
		LastTextString = NewPlayerName;
	}
}

void ANameSwitcher::Interact_Implementation(UPrimitiveComponent* InteractedComponent)
{
	ASwitcherBase::Interact_Implementation(InteractedComponent);
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
				ShowCaret();
				EditableText->OnTextChanged.AddDynamic(this, &ANameSwitcher::PlayerNameTextUpdated);
				EditableText->OnTextCommitted.AddDynamic(this, &ANameSwitcher::PlayerNameTextCommitted);
				EditableText->SetText(TextComponent->GetText());
				EditableText->SetFocus();
			}
		}
	}
}

void ANameSwitcher::PlayerNameTextUpdated(const FText& NewText)
{
	const FString NewTextString = NewText.ToString();
	const int32 NewTextLength = NewText.ToString().Len();
	if (NewTextLength > MaxStringLength)
	{
		const FString CurrentText = CustomTextWidget->EditableTextBox->GetText().ToString();
		CustomTextWidget->EditableTextBox->SetText(FText::FromString(CurrentText.Left(MaxStringLength)));
		return;
	}
	if (TextComponent)
	{
		TextComponent->SetText(NewText);
		if (Caret)
		{
			const int32 LastTextLength = LastTextString.Len();
			const FVector CaretLocation = Caret->GetComponentLocation();
			if (NewTextLength < LastTextLength)
			{
				Caret->SetRelativeLocation(CaretLocation + FVector( 0, TextSpacing, 0));
			}
			else if (NewTextLength > LastTextLength)
			{
				Caret->SetRelativeLocation(CaretLocation - FVector( 0, TextSpacing, 0));
			}
		}
	}
	LastTextString = NewText.ToString();
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
	HideCaret();
}