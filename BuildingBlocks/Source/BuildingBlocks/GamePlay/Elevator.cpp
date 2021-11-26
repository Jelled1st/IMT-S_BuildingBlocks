// Fill out your copyright notice in the Description page of Project Settings.

#include "Elevator.h"
#include "GenericPlatform/GenericPlatformMath.h"
#include "../Core/CoreSystem.h"
#include "../Utilities/Utility.h"


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
	
	UElevatorHandler* elevatorHandler = UCoreSystem::Get().GetElevatorHandler();

	check(elevatorHandler != nullptr);
	elevatorHandler->Register(*this);

	ConvertSoftPtrs();

	check(m_floors.Num() != 0);

	if (startFloor < 0 || startFloor >= m_floors.Num())
	{
		startFloor = 0;
	}

	float startFloorHeight = m_floors[startFloor]->GetActorLocation().Y;

	m_startPosition = m_elevatorPlatform->GetActorLocation();
	m_startPosition.Y = startFloorHeight;
	m_elevatorPlatform->SetActorLocation(m_startPosition);
}

void AElevator::ConvertSoftPtrs()
{
	for (TSoftObjectPtr<AActor> floor : floorsSoftPtr)
	{
		m_floors.Add(UUtility::ConvertSoftPtr(floor));
	}

	m_elevatorPlatform = UUtility::ConvertSoftPtr(elevatorPlatformSoftPtr);

	m_cameraActor = UUtility::ConvertSoftPtr(cameraActorSoftPtr);
}

void AElevator::BeginDestroy()
{
	Super::BeginDestroy();

	if (UCoreSystem::Exists())
	{
		UElevatorHandler* elevatorHandler = UCoreSystem::Get().GetElevatorHandler();

		if (elevatorHandler != nullptr)
		{
			elevatorHandler->Register(*this);
		}
	}
}

// Called every frame
void AElevator::Tick(float deltaTime)
{
	Super::Tick(deltaTime);

	if (m_state != ElevatorState::Idle)
	{
		AActor* floorActor = m_floors[m_desinationIndex];
		FVector destination = floorActor->GetActorLocation();

		FVector pos = m_elevatorPlatform->GetActorLocation();
		float zDiff = destination.Z - pos.Z;
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

		pos.Z += movement;
		m_elevatorPlatform->SetActorLocation(pos);

		m_cameraActor->GetWorldOffset() = pos - m_startPosition;
	}
}

void AElevator::MoveUp(int floorCount)
{
	int dest = m_desinationIndex + floorCount;
	if (dest < m_floors.Num())
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
