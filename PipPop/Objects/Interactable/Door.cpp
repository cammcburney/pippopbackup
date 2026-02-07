// Fill out your copyright notice in the Description page of Project Settings.


#include "Objects/Interactable/Door.h"
#include "Components/BoxComponent.h"
#include "Net/UnrealNetwork.h"

// Sets default values
ADoor::ADoor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;
	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));
	CollisionComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("CollisionComponent"));
	TimelineComponent = CreateDefaultSubobject<UTimelineComponent>(TEXT("TimelineComponent"));
	Mesh = ConstructorHelpers::FObjectFinder<UStaticMesh>(TEXT("StaticMesh'/Engine/BasicShapes/Cube.Cube'")).Object;
	if (Mesh && MeshComponent)
	{
		SetRootComponent(MeshComponent);
		MeshComponent->SetStaticMesh(Mesh);
		if (CollisionComponent)
		{
			CollisionComponent->SetupAttachment(RootComponent);
			CollisionComponent->OnComponentBeginOverlap.AddDynamic(this, &ADoor::OnOverlapBegin);
		}
	}
	bOpened = true;
}

// Called when the game starts or when spawned
void ADoor::BeginPlay()
{
	Super::BeginPlay();
	SetReplicateMovement(true);
	if (Mesh && MeshComponent)
	{
		MeshComponent->SetStaticMesh(Mesh);
	}
}

// Called every frame
void ADoor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ADoor::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	if (HasAuthority())
	{
		ToggleDoor();
	}
	else
	{
		ServerToggleDoor();
	}
}


void ADoor::ToggleDoor()
{
	bOpened = !bOpened;
	OnRep_DoorToggled();
	GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Cyan, FString::Printf(TEXT("Door toggled %d"), bOpened));
}

void ADoor::ServerToggleDoor_Implementation()
{
	ToggleDoor();
}

bool ADoor::ServerToggleDoor_Validate()
{
	return true;
}

void ADoor::OnRep_DoorToggled()
{
	if (bOpened)
	{
		CloseDoor();
	}
	else
	{
		OpenDoor();
	}
}

void ADoor::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ADoor, bOpened)
}


void ADoor::OpenDoor()
{
	SetActorLocation(FVector(200, 0, -100));
}

void ADoor::CloseDoor()
{
	SetActorLocation(FVector(-200, 0, -100));
}