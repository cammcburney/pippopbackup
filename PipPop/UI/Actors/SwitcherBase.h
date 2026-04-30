// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Character/Interfaces/CustomisationInterface.h"
#include "Data/AudioStructs.h"
#include "Text3DComponent.h"
#include "SwitcherBase.generated.h"

class UUserWidget;
class UGameplayStatics;

UCLASS()
class PIPPOP_API ASwitcherBase : public AActor, public ICustomisationInterface
{
	GENERATED_BODY()
	
public:	
	ASwitcherBase();

	UPROPERTY(EditInstanceOnly)
	TObjectPtr<UText3DComponent> TextComponent;
	
	UPROPERTY(EditInstanceOnly)
	TObjectPtr<UStaticMeshComponent> StaticMeshComponent;

	UPROPERTY(EditInstanceOnly)
	FSoundEffect InteractSoundEffect;
	
	virtual void Interact_Implementation(UPrimitiveComponent* InteractedComponent) override;

	virtual void PlaySoundEffect(const FSoundEffect PlayedSoundEffect);
};
