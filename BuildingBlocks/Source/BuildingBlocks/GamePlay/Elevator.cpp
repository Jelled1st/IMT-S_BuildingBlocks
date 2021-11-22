// Fill out your copyright notice in the Description page of Project Settings.


#include "GenericPlatform/GenericPlatformMath.h"
#include "Elevator.h"

// Sets default values
AElevator::AElevator()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
}

// Called when the game starts or when spawned
void AElevator::BeginPlay()
{
	Super::BeginPlay();
	
	FAttachmentTransformRules attachmentRules = FAttachmentTransformRules(EAttachmentRule::KeepWorld, false);

	for (AActor* floor : floors)
	{
		floor->AttachToComponent(RootComponent, attachmentRules);
	}

	for(AActor* actor : actors)
	{
		actor->AttachToComponent(RootComponent, attachmentRules);
	}
}

// Called every frame
void AElevator::Tick(float deltaTime)
{
	Super::Tick(deltaTime);

	if (m_state != ElevatorState::Idle)
	{
		AActor* floorActor = floors[m_desinationIndex];
		FVector destination = floorActor->GetActorLocation();

		FVector plateauPos = elevatorPlateau->GetActorLocation();
		float zDiff = plateauPos.Z - destination.Z;
		float zDiffAbs = FGenericPlatformMath::Abs(zDiff);
		float movement = speed * deltaTime;
		
		bool willOvershoot = movement >= zDiffAbs;
		if (willOvershoot)
		{
			movement = zDiffAbs;
			m_state = ElevatorState::Idle;
			m_currentFloorIndex = m_desinationIndex;
		}

		bool isMovingDown = zDiff < 0;
		if (isMovingDown)
		{
			movement = -movement;
		}

		FVector rootPos = RootComponent->GetComponentLocation();
		rootPos.Z += movement;
		RootComponent->SetWorldLocation(rootPos);
	}
}

void AElevator::MoveUp(int floorCount)
{
	int dest = m_desinationIndex + floorCount;
	if (dest < floors.Num())
	{
		m_desinationIndex = dest;
		m_state = ElevatorState::Moving;
	}
}

void AElevator::MoveDown(int floorCount)
{
	int dest = m_desinationIndex - floorCount;
	if (dest >= 0)
	{
		m_desinationIndex = dest;
		m_state = ElevatorState::Moving;
	}
}
