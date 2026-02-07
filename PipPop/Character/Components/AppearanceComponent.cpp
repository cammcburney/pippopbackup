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
		AppearanceComponents.Add(EAppearance::Head, Head);
	}
	if (USkeletalMeshComponent* Ears = CreateDefaultSubobject<USkeletalMeshComponent>("EarsMesh"))
	{
		AppearanceComponents.Add(EAppearance::Ears, Ears);
	}
	if (USkeletalMeshComponent* LeftEye = CreateDefaultSubobject<USkeletalMeshComponent>("LeftEyeMesh"))
	{
		AppearanceComponents.Add(EAppearance::LeftEye, LeftEye);
	}
	if (USkeletalMeshComponent* RightEye = CreateDefaultSubobject<USkeletalMeshComponent>("RightEyeMesh"))
	{
		AppearanceComponents.Add(EAppearance::RightEye, RightEye);
	}
	if (USkeletalMeshComponent* Mouth = CreateDefaultSubobject<USkeletalMeshComponent>("MouthMesh"))
	{
		AppearanceComponents.Add(EAppearance::Mouth, Mouth);
	}
	if (USkeletalMeshComponent* Torso = CreateDefaultSubobject<USkeletalMeshComponent>("TorsoMesh"))
	{
		AppearanceComponents.Add(EAppearance::Torso, Torso);
	}
	if (USkeletalMeshComponent* Tail = CreateDefaultSubobject<USkeletalMeshComponent>("TailMesh"))
	{
		AppearanceComponents.Add(EAppearance::Tail, Tail);
	}
	if (USkeletalMeshComponent* Legs = CreateDefaultSubobject<USkeletalMeshComponent>("LegsMesh"))
	{
		AppearanceComponents.Add(EAppearance::Legs, Legs);
	}
	if (USkeletalMeshComponent* Accessory = CreateDefaultSubobject<USkeletalMeshComponent>("AccessoryMesh"))
	{
		AppearanceComponents.Add(EAppearance::Accessory, Accessory);
	}
}


// Called when the game starts
void UAppearanceComponent::BeginPlay()
{
	Super::BeginPlay();

	if (!GetWorld()) {return;}
	UPipPopGameInstance* GameInst = Cast<UPipPopGameInstance>(GetWorld()->GetGameInstance());
	if (!GameInst) {return;}
	UAppearanceSubsystem* AppearanceSubsystem = GameInst->GetSubsystem<UAppearanceSubsystem>();
	if (!AppearanceSubsystem) {return;}
	for (const auto& AppearanceElem : AppearanceComponents)
	{
		const FName SectionName = GetSectionName(AppearanceElem.Key);
		if (USkeletalMesh* AppearanceItem = AppearanceSubsystem->LoadAppearanceAsset(SectionName, 0, &FAppearanceInfo::Mesh))
		{
			AppearanceElem.Value->SetSkeletalMesh(AppearanceItem);
			AppearanceElem.Value->SetupAttachment(GetOwner()->GetRootComponent());
			AppearanceElem.Value->RegisterComponent();
			if (UMaterialInterface* MaterialItem = AppearanceSubsystem->LoadAppearanceAsset(SectionName, 0, &FAppearanceInfo::Material))
			{
				AppearanceElem.Value->SetMaterial(0, MaterialItem);
			}
		}
	}
}


// Called every frame
void UAppearanceComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
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

