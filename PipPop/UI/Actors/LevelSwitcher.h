// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Character/Interfaces/CustomisationInterface.h"
#include "Text3DActor.h"
#include "LevelSwitcher.generated.h"

class UUserWidget;

UCLASS()
class PIPPOP_API ALevelSwitcher : public AActor, public ICustomisationInterface
{
	GENERATED_BODY()
	
public:	
	ALevelSwitcher();
	
	UPROPERTY(EditInstanceOnly)
	TObjectPtr<UStaticMesh> StaticMesh;

	UPROPERTY(EditInstanceOnly)
	TObjectPtr<UMaterialInterface> MeshMaterial;

	UPROPERTY(EditInstanceOnly)
	TSubclassOf<UUserWidget> TransitionWidgetClass;

	UPROPERTY()
	TObjectPtr<UStaticMeshComponent> StaticMeshComponent;

	UPROPERTY()
	TObjectPtr<UText3DComponent> TextComponent;
	
protected:
	
	virtual void BeginPlay() override;

public:	

	virtual void Interact_Implementation(UPrimitiveComponent* InteractedComponent) override;
	
	
};
