// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Mode/AppearanceSubsystem.h"
#include "Character/Components/AppearanceComponent.h"
#include "Character/Interfaces/CustomisationInterface.h"
#include "AppearanceSwitcher.generated.h"

class UPipPopGameInstance;
class UAppearanceSubsystem;
class ACustomisationPawn;
class UAppearanceComponent;
class UGameplayStatics;

UCLASS()
class PIPPOP_API AAppearanceSwitcher : public AActor, public ICustomisationInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AAppearanceSwitcher();

	UPROPERTY(EditInstanceOnly)
	float Distance = -100.f;
	
	UPROPERTY(EditInstanceOnly)
	EAppearanceType AppearanceType;

	UPROPERTY(EditInstanceOnly)
	EAppearance AppearanceCategory;
	
	UPROPERTY(EditInstanceOnly)
	FName AppearanceSection;
	
	UPROPERTY(EditInstanceOnly)
	UStaticMesh* StaticMesh;

	UPROPERTY(EditInstanceOnly)
	UMaterialInterface* MeshMaterial;
	
	UPROPERTY(EditInstanceOnly)
	FRotator MeshRotation = FRotator(0, 0, 0);

	UPROPERTY(EditInstanceOnly)
	FVector MeshScale = FVector(1, 1, 1);
	
private:
	
	UPROPERTY()
	int32 Index = 0;

	UPROPERTY()
	int32 MaxIndex = 0;

	UPROPERTY()
	TArray<TObjectPtr<UStaticMeshComponent>> StaticMeshComponents;
	
	
public:	
	
	UAppearanceSubsystem* GetAppearanceSubsystem() const;
	
	USkeletalMesh* GetMeshAsset(UAppearanceSubsystem* AppearanceSubsystem) const;
	
	UMaterialInterface* GetMaterialAsset(UAppearanceSubsystem* AppearanceSubsystem) const;
	
	ACustomisationPawn* GetPlayer() const;
	
	virtual void Interact_Implementation(UPrimitiveComponent* InteractedComponent) override;

	int32 GetNextValidIndex(const int IterateBy) const;

	void SetupActor() const;
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void OnConstruction(const FTransform& Transform) override;
};
