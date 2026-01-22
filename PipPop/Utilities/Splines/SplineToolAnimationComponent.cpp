
#include "SplineToolAnimationComponent.h"
#include "Components/InstancedStaticMeshComponent.h"
#include "Components/TimelineComponent.h"
#include "Components/SplineComponent.h"
#include "SplineTool.h"

void USplineToolAnimationComponent::BeginPlay()
{
	Super::BeginPlay();
	SplineTool = Cast<ASplineTool>(GetOwner());
	if (SplineTool)
	{
		SplineTool->OnSplineToolInitialised.AddDynamic(this, &USplineToolAnimationComponent::InitialiseAnimation);
	}
}

void USplineToolAnimationComponent::InitialiseAnimation()
{
	if (SplineTool && CurveFloat)
	{
		SplineComponent = SplineTool->GetSplineComponent();
		TimelineComponent = SplineTool->GetTimelineComponent();
		if (!SplineComponent || !TimelineComponent) {return;}
		SplineLength = SplineTool->GetSplineLength();
		Spacing = SplineTool->GetSpacing();
		CustomActors = SplineTool->GetCustomActors();
		FOnTimelineFloat TimelineFloatValue;
		if (SplineTool->GetGenerationMethod() == EGenerationMethod::Actor)
		{
			TimelineFloatValue.BindUFunction(this, FName("UpdateActorTimeline"));
			Count = CustomActors.Num();
		}
		else if (SplineTool->GetGenerationMethod() == EGenerationMethod::Generative)
		{
			TimelineFloatValue.BindUFunction(this, FName("UpdateGenerativeTimeline"));
			Count = SplineTool->GetCount();
			const int32 InstanceCount = SplineTool->GetNumberOfInstances(Spacing);
			Spacing = SplineTool->CheckForClosedLoop(InstanceCount);
		}
		TimelineComponent->AddInterpFloat(CurveFloat, TimelineFloatValue);
		FOnTimelineEvent TimelineFinished;
		if (Looping) {TimelineFinished.BindUFunction(this, FName("PlayTimeline"));}
		TimelineComponent->SetTimelineFinishedFunc(TimelineFinished);
		TimelineComponent->SetPlayRate(1.f * PlayRateMultiplier);
	}
	if (PlayAnimationOnSpawn){PlayTimeline();}
}

	

void USplineToolAnimationComponent::PlayTimeline() const
{
	if (!IsValid(TimelineComponent)) {return;}
	TimelineComponent->PlayFromStart();
}

void USplineToolAnimationComponent::UpdateActorTimeline(const float Alpha)
{
	if (!IsValid(TimelineComponent) || !Count || !SplineTool) {return;}
	if (Count)
	{
		for (int i = 0; i < Count; i++)
		{
			float Distance = SplineTool->GetSplinePosition(Alpha) + (i * SplineTool->GetOffset());
			if (Distance > SplineLength) {Distance = FMath::Fmod(Distance, SplineLength);}
			SplineTool->SetSplinePosition(CustomActors[i], Distance);
		}
	}
}

void USplineToolAnimationComponent::UpdateGenerativeTimeline(const float Alpha)
{
	if (!IsValid(TimelineComponent) || !Count || !SplineComponent) {return;}
	int32 CurrentIndex = 0;
	for (UInstancedStaticMeshComponent* InstMeshComp : SplineTool->GetInstancedStaticMeshes())
	{
		if (!InstMeshComp) continue;
		const int32 InstanceCount = InstMeshComp->GetInstanceCount();
		for (int32 i = 0; i < InstanceCount; i++)
		{
			const float Distance = GetDistance(CurrentIndex, Alpha);
			FTransformProperties Transform = SplineTool->GetTransformProperties();
			const FVector Location = SplineComponent->GetLocationAtDistanceAlongSpline(Distance, ESplineCoordinateSpace::World) + Transform.PositionOffset;
			const FRotator Rotation = SplineComponent->GetRotationAtDistanceAlongSpline(Distance, ESplineCoordinateSpace::World) + Transform.RotationOffset;
			FTransform NewTransform(Rotation, Location);
			InstMeshComp->UpdateInstanceTransform(i, NewTransform, true, false);
			CurrentIndex++;
		}
		InstMeshComp->MarkRenderStateDirty();
	}
}

float USplineToolAnimationComponent::GetDistance(const float Index, const float Alpha) const 
{
	float Distance = 0.f;
	switch(AnimationType)
	{
	case EAnimationType::SplineLength:
		{
			if (!SplineTool) {return 0.f;}
			Distance = SplineTool->GetSplinePosition(Alpha) + (Index * Spacing);
			if (Distance > SplineLength) { Distance = FMath::Fmod(Distance, SplineLength); }
			break;
		}
	case EAnimationType::FromStartPoint:
		{
			const float TimelineDistance = Alpha * SplineLength;
			const float ExtendedDistance = Index * Spacing;
			Distance = FMath::Min(ExtendedDistance, TimelineDistance);
			break;
		}
	}
	return Distance;
}
