
#include "SplineTool.h"
#include "Components/InstancedStaticMeshComponent.h"
#include "Components/SplineComponent.h"
#include "Components/TimelineComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Engine/EngineTypes.h"

ASplineTool::ASplineTool()
{
	PrimaryActorTick.bCanEverTick = false;
	SplineComponent = CreateDefaultSubobject<USplineComponent>(TEXT("SplineComponent"));
	SetRootComponent(SplineComponent);
	TimelineComponent = CreateDefaultSubobject<UTimelineComponent>(TEXT("TimelineComponent"));
}

#if WITH_EDITOR
void ASplineTool::PostEditUndo()
{
	Super::PostEditUndo();
	const TArray<UInstancedStaticMeshComponent*> Instances = GatherInstances();
	RemoveInstances(Instances);
	OnConstruction(GetActorTransform());
}

void ASplineTool::PostEditChangeProperty(struct FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);
	const TArray<UInstancedStaticMeshComponent*> Instances = GatherInstances();
	RemoveInstances(Instances);
	OnConstruction(GetActorTransform());
}
#endif

void ASplineTool::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
	if (!IsValid(SplineComponent)) {return;}
	SplineLength = SplineComponent->GetSplineLength();
	if (GenerationMethod == EGenerationMethod::Actor)
	{
		Count = CustomActors.Num();
		if (Count)
		{
			for (int i = 0; i < Count; i++)
			{
				const float Distance = i * Offset;
				SetSplinePosition(CustomActors[i], Distance);
			}
		}
	}
	else if (GenerationMethod == EGenerationMethod::Generative)
	{
		if (SnapPointsToGround) {SnapSplineToGround();}
		if (StaticMeshes.IsEmpty()) {return;}
		ClearGeneratedInstances();
		CreateGeneratedInstances();
		Spacing = GetSpacing();
		const int32 InstanceCount = GetNumberOfInstances(Spacing);
		const float FinalSpacing = CheckForClosedLoop(InstanceCount);
		for (int i = 0; i < InstanceCount; i++)
		{
			UInstancedStaticMeshComponent* InstMeshComp = InstancedStaticMeshComponents[FMath::RandRange(0, InstancedStaticMeshComponents.Num() - 1)];
			const float Distance = i * FinalSpacing;
			FVector Location = SplineComponent->GetLocationAtDistanceAlongSpline(Distance, ESplineCoordinateSpace::Local) + TransformProperties.PositionOffset;
			FRotator Rotation = SplineComponent->GetRotationAtDistanceAlongSpline(Distance, ESplineCoordinateSpace::Local) + TransformProperties.RotationOffset;
			Location += GetRandom3XValues<FVector>(0.f,TransformProperties.PositionRandomisation, 
				TransformProperties.PositionLocks.bLockX,
				TransformProperties.PositionLocks.bLockY,
				TransformProperties.PositionLocks.bLockZ);
			Rotation = FRotator(FQuat(Rotation) * FQuat(GetRandom3XValues<FRotator>(0.f, TransformProperties.RotationRandomisation, 
				TransformProperties.RotationLocks.bLockX,
				TransformProperties.RotationLocks.bLockY,
				TransformProperties.RotationLocks.bLockZ)));
			FVector ScaleMod = InstMeshComp->GetComponentScale() * GetRandom3XValues<FVector>(TransformProperties.Scale, TransformProperties.ScaleRandomisation, 
				TransformProperties.ScaleLocks.bLockX,
				TransformProperties.ScaleLocks.bLockY,
				TransformProperties.ScaleLocks.bLockZ);
			FTransform MyTransform(Rotation, Location, ScaleMod);
			InstMeshComp->AddInstance(MyTransform);
		}
	}
}

void ASplineTool::BeginPlay()
{
	Super::BeginPlay();
	if (SplineComponent) {SplineLength = SplineComponent->GetSplineLength();}
	Spacing = GetSpacing();
	Count = 0;
	for (const UInstancedStaticMeshComponent* InstMeshComp : InstancedStaticMeshComponents)
	{
		Count += InstMeshComp->GetInstanceCount();
	}
	Spacing = CheckForClosedLoop(Count);
	OnSplineToolInitialised.Broadcast();
}

void ASplineTool::SnapSplineToGround()
{
	const int32 Points = SplineComponent->GetNumberOfSplinePoints();
	for (int32 i = 0; i < Points; i++)
	{
		const FVector Start = SplineComponent->GetLocationAtSplinePoint(i, ESplineCoordinateSpace::World);
		const UWorld* const World = GetWorld();
		const FVector End = Start + (FVector::DownVector * SplineMagnetism);
		TArray<AActor*> ActorsToIgnore;
		ActorsToIgnore.Add(this);
		FHitResult OutHit;
		const bool bHit = UKismetSystemLibrary::LineTraceSingle(
				World,
				Start,
				End,
				UEngineTypes::ConvertToTraceType(ECC_Visibility),
				false,
				ActorsToIgnore,
				EDrawDebugTrace::None,
				OutHit,
				true
				);
		if (bHit)
		{
			SplineComponent->SetLocationAtSplinePoint(i, OutHit.ImpactPoint + (OutHit.ImpactNormal * MagnetismOffset), ESplineCoordinateSpace::World);
		}
	}
}

void ASplineTool::ClearGeneratedInstances()
{
	Modify();
	for (UInstancedStaticMeshComponent* InstMeshComp : InstancedStaticMeshComponents)
	{
		if (InstMeshComp)
		{
			InstMeshComp->Modify();
			InstMeshComp->ClearInstances();
			InstMeshComp->UnregisterComponent();
			InstMeshComp->DestroyComponent();
		}
	}
	InstancedStaticMeshComponents.Empty();
}

void ASplineTool::CreateGeneratedInstances()
{
	for (UStaticMesh* Mesh : StaticMeshes)
	{
		UInstancedStaticMeshComponent* InstMeshComp = NewObject<UInstancedStaticMeshComponent>(this, NAME_None, RF_Transactional);
		InstMeshComp->SetStaticMesh(Mesh);
		InstMeshComp->SetupAttachment(SplineComponent);
		InstMeshComp->RegisterComponent();
		InstancedStaticMeshComponents.Add(InstMeshComp);
	}
}

template<typename Type>
Type ASplineTool::GetRandom3XValues(const float InitialValue, const float Randomise, const bool bLockX, const bool bLockY, const bool bLockZ)
{
	const float Min = InitialValue - Randomise;
	const float Max = InitialValue + Randomise;
	const float X = bLockX ? InitialValue : FMath::RandRange(Min, Max);
	const float Y = bLockY ? InitialValue : FMath::RandRange(Min, Max);
	const float Z = bLockZ ? InitialValue : FMath::RandRange(Min, Max);
	return Type(X, Y, Z);
}

FVector ASplineTool::GetLocationAtIndex(const int Index, const float ActorSpacing) const
{
	return SplineComponent->GetLocationAtDistanceAlongSpline((Index * ActorSpacing), ESplineCoordinateSpace::Local);
}

FRotator ASplineTool::GetRotationAtIndex(const int Index, const float ActorSpacing) const
{
	return SplineComponent->GetRotationAtDistanceAlongSpline((Index * ActorSpacing), ESplineCoordinateSpace::Local);
}

float ASplineTool::GetSpacing() const
{
	if (StaticMeshes.IsEmpty()) {return 0.f;}
	float FinalSpacing = 0.f;
	for (const UStaticMesh* Mesh : StaticMeshes)
	{
		if (!IsValid(Mesh)) {continue;}
		FBox BoundingBox = Mesh->GetBoundingBox();
		FQuat Rotation = TransformProperties.RotationOffset.Quaternion();
		FVector Position = TransformProperties.PositionOffset;
		FVector Scale(TransformProperties.Scale);
		BoundingBox = BoundingBox.TransformBy(FTransform(Rotation, Position, Scale));
		const float MeshSpacing = (BoundingBox.Max - BoundingBox.Min).X;
		FinalSpacing = FMath::Max(FinalSpacing, MeshSpacing);
	}
	return FinalSpacing + Offset;
}

float ASplineTool::GetNumberOfInstances(const float ActorSpacing) const
{
	return LimitInstances ? MaxInstances : FMath::FloorToInt(SplineLength / ActorSpacing);
}

float ASplineTool::CheckForClosedLoop(const int32 InstanceCount) const
{
	const bool bClosedLoop = SplineComponent->IsClosedLoop();
	return bClosedLoop ? (SplineLength / InstanceCount) : GetSpacing();
}

float ASplineTool::GetSplinePosition(const float Alpha) const
{
	return FMath::Lerp(0.f, SplineLength, Alpha);
}

void ASplineTool::SetSplinePosition(AActor* Actor, const float Distance) const
{
	if (!IsValid(Actor)) {return;}
	const FVector Location = SplineComponent->GetLocationAtDistanceAlongSpline(Distance, ESplineCoordinateSpace::World);
	const FRotator Rotation = SplineComponent->GetRotationAtDistanceAlongSpline(Distance, ESplineCoordinateSpace::World);
	Actor->SetActorLocationAndRotation(Location, Rotation);
}

TArray<UInstancedStaticMeshComponent*> ASplineTool::GatherInstances() const
{
	TArray<UInstancedStaticMeshComponent*> Instances;
	GetComponents(Instances);
	return Instances;
}

void ASplineTool::RemoveInstances(TArray<UInstancedStaticMeshComponent*> Instances)
{
	for (UInstancedStaticMeshComponent* Instance : Instances)
	{
		Instance->ClearInstances();
		Instance->UnregisterComponent();
		Instance->DestroyComponent();
	}
}