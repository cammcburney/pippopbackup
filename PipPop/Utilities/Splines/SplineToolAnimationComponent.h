
#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "SplineToolAnimationComponent.generated.h"

class ASplineTool;
class USplineComponent;
class UTimelineComponent;
class UInstancedStaticMeshComponent;
class AActor;
enum class EAnimationType : uint8;

UCLASS(Blueprintable, meta=(BlueprintSpawnableComponent))
class PIPPOP_API USplineToolAnimationComponent : public UActorComponent
{
	GENERATED_BODY()

public:

	float GetDistance(const float Index, const float Alpha) const;

	UFUNCTION()
	void InitialiseAnimation();
	
protected:

	virtual void BeginPlay() override;
	
	UFUNCTION(BlueprintCallable)
	void PlayTimeline() const;

	UFUNCTION(BlueprintCallable)
	void UpdateActorTimeline(const float Alpha);

	UFUNCTION(BlueprintCallable)
	void UpdateGenerativeTimeline(const float Alpha);

private:

	UPROPERTY()
	TObjectPtr<ASplineTool> SplineTool;
	
	UPROPERTY()
	TObjectPtr<UTimelineComponent> TimelineComponent;

	UPROPERTY()
	TObjectPtr<USplineComponent> SplineComponent;

	UPROPERTY(EditAnywhere, Category="Spline|Animation")
	EAnimationType AnimationType;
	
	UPROPERTY(EditAnywhere, Category="Spline|Animation")
	bool PlayAnimationOnSpawn = false;

	UPROPERTY(EditAnywhere, Category="Spline|Animation")
	bool Looping = false;

	UPROPERTY(EditAnywhere, Category="Spline|Animation")
	TObjectPtr<UCurveFloat> CurveFloat;

	UPROPERTY(EditAnywhere, Category="Spline|Animation")
	float PlayRateMultiplier = 1;

	TArray<TObjectPtr<AActor>> CustomActors;
	
	int32 Count = 0;

	float Spacing = 0.f;

	float SplineLength = 0.f;
};