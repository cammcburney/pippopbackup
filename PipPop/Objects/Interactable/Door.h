// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/TimelineComponent.h"
#include "GameFramework/Actor.h"
#include "Door.generated.h"

class UBoxComponent;
UCLASS()
class PIPPOP_API ADoor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ADoor();

	void ToggleDoor();

	UFUNCTION(Server, Reliable, WithValidation)
	void ServerToggleDoor();
	void ServerToggleDoor_Implementation();
	bool ServerToggleDoor_Validate();
	
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere)
	TObjectPtr<UStaticMesh> Mesh;

	UPROPERTY(EditAnywhere)
	TObjectPtr<UBoxComponent> CollisionComponent;
	
private:
	
	UPROPERTY(ReplicatedUsing = OnRep_DoorToggled)
	bool bOpened;
	
	UPROPERTY()
	TObjectPtr<UStaticMeshComponent> MeshComponent;

	UPROPERTY()
	TObjectPtr<UTimelineComponent> TimelineComponent;
	
	
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);

	UFUNCTION()
	void OnRep_DoorToggled();
	
	UFUNCTION()
	void OpenDoor();

	UFUNCTION()
	void CloseDoor();
	
};
