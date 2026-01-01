// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/TextBlock.h"
#include "Widgets/CustomisationManager.h"
#include "Character/Components/AppearanceComponent.h"
#include "SwitchMenuItem.generated.h"


class UAppearanceSubsystem;
class UButton;
class UTextBlock;

/**
 * 
 */
UCLASS()
class PIPPOP_API USwitchMenuItem : public UUserWidget
{
	GENERATED_BODY()
	
private:

	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UTextBlock> ItemName;
	
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UButton> LeftButton;
	
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UButton> RightButton;

	UPROPERTY()
	TObjectPtr<USkeletalMeshComponent> SkeletalMeshComponent;

	UPROPERTY()
	TObjectPtr<UCustomisationManager> Owner;
	
	UPROPERTY()
	FName AppearanceSection;

	UPROPERTY()
	int32 Index = 0;
	
public:

	virtual void NativeConstruct() override;

	void SetSection(const FName NewSection) {AppearanceSection = NewSection; GEngine->AddOnScreenDebugMessage(-1, 1.5, FColor::Red, FString::Printf(TEXT("ADD SECTION")));}

	void SetAppearanceReference(USkeletalMeshComponent* NewSkelMeshComp) {SkeletalMeshComponent = NewSkelMeshComp;GEngine->AddOnScreenDebugMessage(-1, 1.5, FColor::Red, FString::Printf(TEXT("ADD SEKELTALSECTION")));}

	void SetManager(UCustomisationManager* NewManager) {Owner = NewManager;}

	void SetItemNameByCategory(const EAppearance Category) const {if (ItemName) {ItemName->SetText(FText::AsCultureInvariant(UEnum::GetValueAsString(Category)));}}

	void SetItemName(const FName NewName) const {ItemName->SetText(FText::FromName(NewName));}
	
private:
	
	UFUNCTION()
	void OnLeftClick() {NextItem(1);};

	UFUNCTION()
	void OnRightClick() {NextItem(-1);}
	
	void NextItem(const int32 Iteration);
	
};
