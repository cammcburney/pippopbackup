// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Actors/AppearanceSwitcher.h"
#include "Character/CustomisationPawn.h"
#include "Kismet/GameplayStatics.h"
#include "Mode/PipPopGameInstance.h"

AAppearanceSwitcher::AAppearanceSwitcher()
{
	PrimaryActorTick.bCanEverTick = false;
	
	const FName ComponentName = FName(*FString::Printf(TEXT("LeftComponent")));
	UStaticMeshComponent* LeftComponent = CreateDefaultSubobject<UStaticMeshComponent>(ComponentName);
	check(LeftComponent)
	LeftComponent->SetMobility(EComponentMobility::Movable);
	LeftComponent->SetupAttachment(RootComponent);
	StaticMeshComponents.Add(LeftComponent);
	
	const FName ComponentName2 = FName(*FString::Printf(TEXT("RightComponent")));
	UStaticMeshComponent* RightComponent = CreateDefaultSubobject<UStaticMeshComponent>(ComponentName2);
	check(RightComponent)
	RightComponent->SetMobility(EComponentMobility::Movable);
	RightComponent->SetupAttachment(RootComponent);
	StaticMeshComponents.Add(RightComponent);
}

void AAppearanceSwitcher::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
	SetupActor();
}

void AAppearanceSwitcher::BeginPlay()
{
	Super::BeginPlay();
	UAppearanceSubsystem* Subsystem = GetAppearanceSubsystem();
	const FName SectionName = AppearanceUtils::GetSectionName(AppearanceCategory);
	if (Subsystem && !SectionName.IsNone())
	{
		MaxIndex = Subsystem->GetSectionLength(SectionName, AppearanceType);
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
	GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Green, TEXT("CHANGE SOMETHING"));
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
			UPipPopGameInstance* GameInst = Cast<UPipPopGameInstance>(GetWorld()->GetGameInstance());
			if (!GameInst) {return;}
			FPlayerSaveData PlayerSaveData = GameInst->LoadPlayerSaveData();
			switch (AppearanceType)
			{
			case EAppearanceType::Mesh:
				{
					USkeletalMesh* Mesh = GetMeshAsset();
					SkeletalMeshComponent->SetSkeletalMesh(Mesh);
					PlayerSaveData.SetAppearanceMesh(AppearanceCategory, Index);
					break;
				}
			case EAppearanceType::Material:
				{
					UMaterialInterface* Material = GetMaterialAsset();
					SkeletalMeshComponent->SetMaterial(MaterialIndex, Material);
					PlayerSaveData.SetAppearanceMaterial(AppearanceCategory, Index);
					break;
				}
			}
			GameInst->SetPlayerSaveData(PlayerSaveData);
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

USkeletalMesh* AAppearanceSwitcher::GetMeshAsset() const
{
	UAppearanceSubsystem* Subsystem = GetAppearanceSubsystem();
	if (!Subsystem) {return nullptr;}
	const FName SectionName = AppearanceUtils::GetSectionName(AppearanceCategory);
	return Subsystem->LoadAppearanceAsset(SectionName, Index, &FAppearanceInfo::Mesh);
}

UMaterialInterface* AAppearanceSwitcher::GetMaterialAsset() const
{
	UAppearanceSubsystem* Subsystem = GetAppearanceSubsystem();
	if (!Subsystem) {return nullptr;}
	const FName SectionName = AppearanceUtils::GetSectionName(AppearanceCategory);
	return Subsystem->LoadAppearanceAsset(SectionName, Index, &FAppearanceInfo::Material);
}

void AAppearanceSwitcher::SetupActor()
{
	if (!StaticMeshComponents.IsEmpty() && StaticMesh && MeshMaterial)
	{
		for (int32 i = 0; i < StaticMeshComponents.Num(); i++)
		{
			if (!StaticMeshComponents.IsValidIndex(i)) {return;}
			if (UStaticMeshComponent* Component = StaticMeshComponents[i])
			{
				Component->SetStaticMesh(StaticMesh);
				Component->SetMaterial(0, MeshMaterial);
				Component->SetRenderCustomDepth(true);
				Component->SetCustomDepthStencilValue(1);
				if (i == 0)
				{
					MainComponentLocation = Component->GetRelativeLocation();
				}
				if (i == 1)
				{
					Component->SetRelativeLocation(FVector(0, Distance, 0) + MainComponentLocation);
				}
			}
		}
	}
}
