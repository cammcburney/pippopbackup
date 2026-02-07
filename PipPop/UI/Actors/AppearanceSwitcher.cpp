// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Actors/AppearanceSwitcher.h"
#include "Character/CustomisationPawn.h"
#include "Kismet/GameplayStatics.h"
#include "Mode/PipPopGameInstance.h"

AAppearanceSwitcher::AAppearanceSwitcher()
{
	PrimaryActorTick.bCanEverTick = false;
	USceneComponent* Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent = Root;
	const FName ComponentName = FName(*FString::Printf(TEXT("Component 1")));
	UStaticMeshComponent* Component = CreateDefaultSubobject<UStaticMeshComponent>(ComponentName);
	Component->SetMobility(EComponentMobility::Movable);
	Component->SetupAttachment(RootComponent);
	StaticMeshComponents.Add( Component);
	const FName ComponentName2 = FName(*FString::Printf(TEXT("Component 2")));
	UStaticMeshComponent* Component2 = CreateDefaultSubobject<UStaticMeshComponent>(ComponentName2);
	Component->SetMobility(EComponentMobility::Movable);
	Component->SetupAttachment(RootComponent);
	StaticMeshComponents.Add( Component2);
}

void AAppearanceSwitcher::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
}

void AAppearanceSwitcher::BeginPlay()
{
	Super::BeginPlay();
	UAppearanceSubsystem* Subsystem = GetAppearanceSubsystem();
	if (Subsystem && !AppearanceSection.IsNone())
	{
		MaxIndex = Subsystem->GetSectionLength(AppearanceSection, AppearanceType);
	}
	SetupActor();
}

UAppearanceSubsystem* AAppearanceSwitcher::GetAppearanceSubsystem() const
{
	if (const UWorld* World = GetWorld())
	{
		if (const UPipPopGameInstance* GameInst = Cast<UPipPopGameInstance>(World->GetGameInstance()))
		{
			return GameInst->GetSubsystem<UAppearanceSubsystem>();
		}
	}
	return nullptr;
}

ACustomisationPawn* AAppearanceSwitcher::GetPlayer() const
{
	if (ACustomisationPawn* PlayerPawn = Cast<ACustomisationPawn>(UGameplayStatics::GetPlayerPawn(this, 0)))
	{
		return PlayerPawn;
	}
	return nullptr;
}


void AAppearanceSwitcher::Interact_Implementation(UPrimitiveComponent* InteractedComponent)
{
	UAppearanceSubsystem* Subsystem = GetAppearanceSubsystem();
	int32 IterateBy = 0;
	for (int32 i = 0; i < StaticMeshComponents.Num(); i++)
	{
		if (!StaticMeshComponents[i]) {return;}
		if (StaticMeshComponents[i] == InteractedComponent)
		{
			IterateBy = (i == 0) ? -1 : 1;
			break;
		}
	}
	Index = GetNextValidIndex(IterateBy);
	if (const ACustomisationPawn* Pawn = GetPlayer())
	{
		if (const UAppearanceComponent* Appearance = Pawn->GetAppearance())
		{
			USkeletalMeshComponent* SkeletalMeshComponent = Appearance->GetSkeletalMeshComponent(AppearanceCategory);
			switch (AppearanceType)
			{
			case EAppearanceType::Mesh:
				{
					USkeletalMesh* Mesh = GetMeshAsset(Subsystem);
					SkeletalMeshComponent->SetSkeletalMesh(Mesh);
					break;
				}
			case EAppearanceType::Material:
				{
					UMaterialInterface* Material = GetMaterialAsset(Subsystem);
					SkeletalMeshComponent->SetMaterial(0, Material);
					break;
				}
			}
		}
	}
}

int32 AAppearanceSwitcher::GetNextValidIndex(const int IterateBy) const
{
	const int32 NextIndex = Index + IterateBy;
	if (NextIndex < 0)
	{
		return MaxIndex - 1;
	}
	if (NextIndex >= MaxIndex)
	{
		return 0;
	}
	return NextIndex;
}

USkeletalMesh* AAppearanceSwitcher::GetMeshAsset(UAppearanceSubsystem* AppearanceSubsystem) const
{
	if (!AppearanceSubsystem) {return nullptr;}
	return AppearanceSubsystem->LoadAppearanceAsset(AppearanceSection, Index, &FAppearanceInfo::Mesh);
}

UMaterialInterface* AAppearanceSwitcher::GetMaterialAsset(UAppearanceSubsystem* AppearanceSubsystem) const
{
	if (!AppearanceSubsystem) {return nullptr;}
	return AppearanceSubsystem->LoadAppearanceAsset(AppearanceSection, Index, &FAppearanceInfo::Material);
}

void AAppearanceSwitcher::SetupActor() const
{
	if (!StaticMeshComponents.IsEmpty() && StaticMesh && MeshMaterial)
	{
		for (int32 i = 0; i < StaticMeshComponents.Num(); i++)
		{
			if (UStaticMeshComponent* Component = StaticMeshComponents[i])
			{
				Component->SetStaticMesh(StaticMesh);
				Component->SetMaterial(0, MeshMaterial);
				Component->SetRenderCustomDepth(true);
				Component->SetCustomDepthStencilValue(1);
				if (i == 1)
				{
					Component->SetRelativeLocation(FVector(0, Distance, 0));
				}
			}
		}
	}
}
