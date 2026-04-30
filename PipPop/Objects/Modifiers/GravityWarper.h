// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GravityWarper.generated.h"

class UArrowComponent;

UCLASS(Blueprintable)
class PIPPOP_API AGravityWarper : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AGravityWarper();

	UPROPERTY(EditAnywhere)
	TObjectPtr<UShapeComponent> GravityRadiusComponent;
	
	UPROPERTY(EditAnywhere)
	TObjectPtr<UArrowComponent> ArrowComponent;
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION()
	virtual void OnComponentOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	virtual void EndComponentOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
	
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
