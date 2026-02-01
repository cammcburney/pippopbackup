// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Character/Interfaces/CustomisationInterface.h"
#include "Text3DComponent.h"
#include "SwitcherBase.generated.h"

class UUserWidget;

UCLASS()
class PIPPOP_API ASwitcherBase : public AActor, public ICustomisationInterface
{
	GENERATED_BODY()
	
public:	
	ASwitcherBase();
	
	UPROPERTY(EditInstanceOnly)
	TObjectPtr<UStaticMesh> StaticMesh;

	UPROPERTY(EditInstanceOnly)
	TObjectPtr<UMaterialInterface> MeshMaterial;

	UPROPERTY(EditInstanceOnly)
	TObjectPtr<UText3DComponent> TextComponent;

	UPROPERTY(EditInstanceOnly)
	TObjectPtr<UFont> MenuFont;
	
private:
	
	UPROPERTY(EditInstanceOnly)
	TObjectPtr<UStaticMeshComponent> StaticMeshComponent;
	
protected:
	
	virtual void BeginPlay() override;

public:	

	void LoadMesh() const;
	
	virtual void Interact_Implementation(UPrimitiveComponent* InteractedComponent) override;


	
};
