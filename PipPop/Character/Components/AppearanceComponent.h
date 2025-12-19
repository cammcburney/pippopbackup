// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "AppearanceComponent.generated.h"

class USkeletalMeshComponent;

UENUM(BlueprintType)
enum class EAppearance : uint8
{
	Head UMETA(DisplayName = "Head"),
	Ears UMETA(DisplayName = "Ears"),
	LeftEye UMETA(DisplayName = "LeftEye"),
	RightEye UMETA(DisplayName = "RightEye"),
	Mouth UMETA(DisplayName = "Mouth"),
	Torso UMETA(DisplayName = "Torso"),
	Legs UMETA(DisplayName = "Legs"),
	Tail UMETA(DisplayName = "Tail"),
	Accessory UMETA(DisplayName = "Accessory")
};
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PIPPOP_API UAppearanceComponent : public UActorComponent
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this component's properties
	UAppearanceComponent();
	
	TMap<EAppearance, USkeletalMeshComponent*> GetAppearanceMap() {return AppearanceComponents;}
	
protected:
	
	UPROPERTY()
	TMap<EAppearance, USkeletalMeshComponent*> AppearanceComponents;
	
protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	USkeletalMeshComponent* GetSkeletalMeshComponent(const EAppearance Section) const {if (AppearanceComponents.Contains(Section)) {return AppearanceComponents[Section];} return nullptr;}
	
public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	static FName GetSectionName(EAppearance AppearanceEnum) {return FName(*FString::FromInt(static_cast<int32>(AppearanceEnum)));}
};
