
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SplineTool.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnSplineToolInitialised);

class UStaticMesh;
class UInstancedStaticMeshComponent;
class USplineComponent;
class UTimelineComponent;
class AActor;

UENUM(BlueprintType)
enum class EGenerationMethod : uint8 {
	Generative = 0 UMETA(DisplayName = "GENERATIVE"),
	Actor = 1  UMETA(DisplayName = "ACTOR")
};

UENUM(BlueprintType)
enum class EAnimationType : uint8 {
	SplineLength = 0 UMETA(DisplayName = "SPLINE LENGTH"),
	FromStartPoint = 1 UMETA(DisplayName = "FROM START POINT"),
};

USTRUCT(BlueprintType)
struct FAxisLocks
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	bool bLockX = false;
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	bool bLockY = false;

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	bool bLockZ = false;
	
};

USTRUCT(BlueprintType)
struct FTransformProperties
{
	
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(ClampMin = 0.f, ClampMax = 1024.f))
	float Scale = 1.f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float ScaleRandomisation = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FAxisLocks ScaleLocks;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector PositionOffset = FVector::ZeroVector;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float PositionRandomisation = 0.f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FAxisLocks PositionLocks;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FRotator RotationOffset = FRotator::ZeroRotator;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float RotationRandomisation = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FAxisLocks RotationLocks;
	
};

UCLASS(Blueprintable)
class PIPPOP_API ASplineTool : public AActor
{
	GENERATED_BODY()
	
public:	

	ASplineTool();
	
	void ClearGeneratedInstances();

	void CreateGeneratedInstances();
	
	float GetSplinePosition(const float Alpha) const;
	
	void SetSplinePosition(AActor* Actor, const float Distance) const;

	float GetSplineLength() const {return SplineLength;}
	
	float GetSpacing() const;
	
	float CheckForClosedLoop(const int32 InstanceCount) const;
	
	float GetNumberOfInstances(const float ActorSpacing) const;

	float GetOffset() const {return Offset;}

	int32 GetCount() const {return Count;}
	
	FVector GetLocationAtIndex(const int Index, const float ActorSpacing) const;

	FRotator GetRotationAtIndex(const int Index, const float ActorSpacing) const;

	template<typename Type>
	static Type GetRandom3XValues(const float InitialValue, const float Randomise, const bool bLockX = false, const bool bLockY = false, const bool bLockZ = false);

	UTimelineComponent* GetTimelineComponent() {if (TimelineComponent) {return TimelineComponent;} else {return nullptr;}}

	USplineComponent* GetSplineComponent() {if (SplineComponent) {return SplineComponent;} else {return nullptr;}}

	EGenerationMethod GetGenerationMethod() const {return GenerationMethod;}

	FTransformProperties GetTransformProperties() const {return TransformProperties;}
	
	TArray<TObjectPtr<UInstancedStaticMeshComponent>> GetInstancedStaticMeshes() {return InstancedStaticMeshComponents;}

	TArray<TObjectPtr<AActor>> GetCustomActors() {return CustomActors;}
	
	UPROPERTY(BlueprintAssignable)
	FOnSplineToolInitialised OnSplineToolInitialised;
	
protected:
	
	virtual void OnConstruction(const FTransform& Transform) override;
	
	virtual void BeginPlay() override;
	
private:
	
	UPROPERTY(EditAnywhere, Category="Spline")
	TObjectPtr<USplineComponent> SplineComponent;

	UPROPERTY(EditAnywhere, Category="Spline")
	EGenerationMethod GenerationMethod;
	
	UPROPERTY(EditAnywhere, Category="Spline|Position")
	float Offset = 100.f;
	
	UPROPERTY(EditAnywhere, Category="Spline|Position")
	bool SnapPointsToGround = false;

	UPROPERTY(EditAnywhere, Category="Spline|Position")
	float SplineMagnetism = 300.f;

	UPROPERTY(EditAnywhere, Category="Spline|Position")
	float MagnetismOffset = 0.f;
	
	UPROPERTY(EditAnywhere, Category="Spline|CustomActors", meta=(EditCondition="GenerationMethod == EGenerationMethod::Actor", EditConditionHides))
	TArray<TObjectPtr<AActor>> CustomActors;
	
	UPROPERTY(EditAnywhere, Category="Spline|GenerativeMesh", meta=(EditCondition="GenerationMethod == EGenerationMethod::Generative", EditConditionHides))
	TArray<TObjectPtr<UStaticMesh>> StaticMeshes;
	
	UPROPERTY(EditAnywhere, Category="Spline|GenerativeMesh", meta=(EditCondition="GenerationMethod == EGenerationMethod::Generative", EditConditionHides))
	FTransformProperties TransformProperties;
	
	UPROPERTY(EditAnywhere, Category="Spline|GenerativeMesh|Instances", meta=(EditCondition="GenerationMethod == EGenerationMethod::Generative", EditConditionHides))
	bool LimitInstances = false;
	
	UPROPERTY(EditAnywhere, Category="Spline|GenerativeMesh|Instances", meta=(ClampMin = 1, ClampMax=1024, EditCondition="GenerationMethod == EGenerationMethod::Generative", EditConditionHides))
	uint32 MaxInstances = 1;

	int32 Count = 0;

	float Spacing = 0.f;

	float SplineLength = 0.f;
	
	UPROPERTY()
	TObjectPtr<UTimelineComponent> TimelineComponent;
	
	UPROPERTY()
	TArray<TObjectPtr<UInstancedStaticMeshComponent>> InstancedStaticMeshComponents;
	
	void SnapSplineToGround();

	TArray<UInstancedStaticMeshComponent*> GatherInstances() const;
	
	static void RemoveInstances(TArray<UInstancedStaticMeshComponent*> Instances);
	
#if WITH_EDITOR
	virtual void PostEditUndo() override;
	virtual void PostEditChangeProperty(struct FPropertyChangedEvent& PropertyChangedEvent) override;
#endif
};
