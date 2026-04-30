// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Data/AppearanceStructs.h"
#include "AppearanceComponent.generated.h"

class USkeletalMeshComponent;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class PIPPOP_API UAppearanceComponent : public UActorComponent
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this component's properties
	UAppearanceComponent();
	
	TMap<EAppearance, USkeletalMeshComponent*> GetAppearanceMap() {return AppearanceComponents;}

protected:
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Appearance")
	TMap<EAppearance, USkeletalMeshComponent*> AppearanceComponents;
	
	// Called when the game starts
	virtual void BeginPlay() override;
	
public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	USkeletalMeshComponent* GetSkeletalMeshComponent(const EAppearance Section) const {if (AppearanceComponents.Contains(Section)) {return AppearanceComponents[Section];} return nullptr;}

	void SetAppearanceMesh(const EAppearance Appearance, USkeletalMesh* NewMesh);

	void SetAppearanceMaterial (const EAppearance Appearance, UMaterialInterface* NewMaterial);
};
