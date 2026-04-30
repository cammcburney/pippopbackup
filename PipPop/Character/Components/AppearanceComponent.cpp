// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Components/AppearanceComponent.h"
#include "Mode/Subsystem/AppearanceSubsystem.h"
#include "Mode/PipPopGameInstance.h"

// Sets default values for this component's properties
UAppearanceComponent::UAppearanceComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	
	PrimaryComponentTick.bCanEverTick = true;
	if (USkeletalMeshComponent* Head = CreateDefaultSubobject<USkeletalMeshComponent>("HeadMesh"))
	{
		AppearanceComponents.Add(EAppearance::HEAD, Head);
	}
	if (USkeletalMeshComponent* Ears = CreateDefaultSubobject<USkeletalMeshComponent>("EarsMesh"))
	{
		AppearanceComponents.Add(EAppearance::EARS, Ears);
	}
	if (USkeletalMeshComponent* Eyes = CreateDefaultSubobject<USkeletalMeshComponent>("EyesMesh"))
	{
		AppearanceComponents.Add(EAppearance::EYES, Eyes);
	}
	if (USkeletalMeshComponent* Mouth = CreateDefaultSubobject<USkeletalMeshComponent>("MouthMesh"))
	{
		AppearanceComponents.Add(EAppearance::MOUTH, Mouth);
	}
	if (USkeletalMeshComponent* Torso = CreateDefaultSubobject<USkeletalMeshComponent>("TorsoMesh"))
	{
		AppearanceComponents.Add(EAppearance::TORSO, Torso);
	}
	if (USkeletalMeshComponent* Tail = CreateDefaultSubobject<USkeletalMeshComponent>("TailMesh"))
	{
		AppearanceComponents.Add(EAppearance::TAIL, Tail);
	}
	if (USkeletalMeshComponent* Legs = CreateDefaultSubobject<USkeletalMeshComponent>("LegsMesh"))
	{
		AppearanceComponents.Add(EAppearance::LEGS, Legs);
	}
	if (USkeletalMeshComponent* Hat = CreateDefaultSubobject<USkeletalMeshComponent>("HatMesh"))
	{
		AppearanceComponents.Add(EAppearance::HAT, Hat);
	}
	if (USkeletalMeshComponent* Accessory1 = CreateDefaultSubobject<USkeletalMeshComponent>("AccessoryMesh1"))
	{
		AppearanceComponents.Add(EAppearance::ACCESSORY_1, Accessory1);
	}
	if (USkeletalMeshComponent* Accessory2 = CreateDefaultSubobject<USkeletalMeshComponent>("AccessoryMesh2"))
	{
		AppearanceComponents.Add(EAppearance::ACCESSORY_2, Accessory2);
	}
}


// Called when the game starts
void UAppearanceComponent::BeginPlay()
{
	Super::BeginPlay();
	
	if (!GetWorld()) {return;}
	UPipPopGameInstance* GameInst = Cast<UPipPopGameInstance>(GetWorld()->GetGameInstance());
	if (!GameInst) {return;}
	FPlayerSaveData PlayerSaveData = GameInst->LoadPlayerSaveData();
	UAppearanceSubsystem* AppearanceSubsystem = GameInst->GetSubsystem<UAppearanceSubsystem>();
	if (!AppearanceSubsystem) {return;}
	for (EAppearance Appearance : TEnumRange<EAppearance>())
	{
		const FName RowName = AppearanceUtils::GetSectionName(Appearance);
		const int32 MeshIndex = PlayerSaveData.GetMeshIndex(Appearance);
		const int32 MaterialIndex = PlayerSaveData.GetMaterialIndex(Appearance);
		if (USkeletalMesh* AppearanceItem = AppearanceSubsystem->LoadAppearanceAsset(RowName, MeshIndex, &FAppearanceInfo::Mesh))
		{
			SetAppearanceMesh(Appearance, AppearanceItem);
		}
		if (UMaterialInterface* MaterialItem = AppearanceSubsystem->LoadAppearanceAsset(RowName, MaterialIndex, &FAppearanceInfo::Material))
		{
			SetAppearanceMaterial(Appearance, MaterialItem);
		}
	}
}


// Called every frame
void UAppearanceComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	for (const auto& AppearanceElem : AppearanceComponents)
	{
		if (USkeletalMeshComponent* AppearanceItem = AppearanceElem.Value)
		{
			if (AppearanceItem)
			{
				FRotator Rotation = AppearanceElem.Value->GetComponentRotation();
				AppearanceElem.Value->SetWorldRotation(Rotation + FRotator(0,.3f,0));
			}
		}
	}
}

void UAppearanceComponent::SetAppearanceMesh(const EAppearance Appearance, USkeletalMesh* NewMesh)
{
	if (AppearanceComponents.Contains(Appearance))
	{
		AppearanceComponents[Appearance]->SetSkeletalMesh(NewMesh);
	}
}

void UAppearanceComponent::SetAppearanceMaterial(const EAppearance Appearance, UMaterialInterface* NewMaterial)
{
	if (AppearanceComponents.Contains(Appearance))
	{
		AppearanceComponents[Appearance]->SetMaterial(0, NewMaterial);
	}
}

