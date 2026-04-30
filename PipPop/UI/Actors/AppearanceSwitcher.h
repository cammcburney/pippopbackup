// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Mode/Subsystem/AppearanceSubsystem.h"
#include "Character/Components/AppearanceComponent.h"
#include "Character/Interfaces/CustomisationInterface.h"
#include "AppearanceSwitcher.generated.h"

class UPipPopGameInstance;
class UAppearanceSubsystem;
class ACustomisationPawn;
class UAppearanceComponent;
class UGameplayStatics;

UCLASS(Blueprintable)
class PIPPOP_API AAppearanceSwitcher : public AActor, public ICustomisationInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AAppearanceSwitcher();

	UPROPERTY(EditAnywhere)
	float Distance = -100.f;
	
	UPROPERTY(EditAnywhere)
	EAppearanceType AppearanceType;

	UPROPERTY(EditAnywhere)
	EAppearance AppearanceCategory;
	
	UPROPERTY(EditAnywhere)
	UStaticMesh* StaticMesh;

	UPROPERTY(EditAnywhere)
	UMaterialInterface* MeshMaterial;

	UPROPERTY(EditAnywhere)
	int32 MaterialIndex;
	
private:

	FVector MainComponentLocation = FVector::ZeroVector;
	
	UPROPERTY()
	int32 Index = 0;

	UPROPERTY()
	int32 MaxIndex = 0;

	UPROPERTY()
	TArray<TObjectPtr<UStaticMeshComponent>> StaticMeshComponents;
	
	
public:	
	
	UAppearanceSubsystem* GetAppearanceSubsystem() const;
	
	USkeletalMesh* GetMeshAsset() const;
	
	UMaterialInterface* GetMaterialAsset() const;
	
	ACustomisationPawn* GetPlayer() const;
	
	virtual void Interact_Implementation(UPrimitiveComponent* InteractedComponent) override;

	int32 GetNextValidIndex(const int IterateBy) const;

	void SetupActor();
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void OnConstruction(const FTransform& Transform) override;
};
