// Fill out your copyright notice in the Description page of Project Settings.

#include "Elevator.h"
#include "GenericPlatform/GenericPlatformMath.h"
#include "../Core/CoreSystem.h"
#include "ElevatorChildComponent.h"
#include "../Utilities/Utility.h"


// Sets default values
AElevator::AElevator() : AModularObject()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
}

// Called when the game starts or when spawned
void AElevator::BeginPlay()
{
	Super::BeginPlay();
	
	if (UCoreSystem::Exists())
	{
		UEventSystem* eventSystem = UCoreSystem::Get().GetEventSystem();
		check(eventSystem != nullptr);
		eventSystem->CallElevatorSpawned(this);
	}

	ConvertSoftPtrs();

	check(m_floors.Num() != 0);

	if (startFloor < 0 || startFloor >= m_floors.Num())
	{
		startFloor = 0;
	}

	m_startPosition = m_elevatorPlatform->GetActorLocation();

	SetupParameter(speed.acceleration, "Speed.acceleration");
	SetupParameter(speed.maxSpeed, "speed.maxSpeed");
	SetupParameter(speed.slowedMinSpeed, "speed.slowedMinSpeed");
	SetupParameter(speed.slowDownDistance, "speed.slowDownDistance");
	SetupParameter(speed.friction, "speed.friction");

	SetupParameter(m_moveUp, "elevator.MoveUp");
	SetupParameter(m_moveDown, "elevator.MoveDown");
}

void AElevator::ConvertSoftPtrs()
{
	for (TSoftObjectPtr<AActor> floor : floorsSoftPtr)
	{
		m_floors.Add(UUtility::ConvertSoftPtr(floor));
	}

	m_elevatorPlatform = UUtility::ConvertSoftPtr(elevatorPlatformSoftPtr);
}

void AElevator::BeginDestroy()
{
	Super::BeginDestroy();
}

// Called every frame
void AElevator::Tick(float deltaTime)
{
	Super::Tick(deltaTime);

	if (m_moveUp)
	{
		m_moveUp = false;
		MoveUp(1);
	}

	if (m_moveDown)
	{
		m_moveDown = false;
		MoveDown(1);
	}

	if (m_state != ElevatorState::Idle)
	{
		AActor* floorActor = m_floors[m_desinationIndex];
		FVector destination = floorActor->GetActorLocation();

		FVector position = m_elevatorPlatform->GetActorLocation();
		FVector newPosition;

		if (Move(position, newPosition, destination, deltaTime))
		{
			m_state = ElevatorState::Idle;
			m_currentFloorIndex = m_desinationIndex;
		}

		m_elevatorPlatform->SetActorLocation(newPosition);

		for (UElevatorChildComponent* child : m_children)
		{
			child->GetWorldOffset() = newPosition - m_startPosition;
		}
	}
}

bool AElevator::Move(const FVector& position, FVector& outPosition, const FVector& destination, float deltaTime)
{
	outPosition = position;
	bool isMovementDone = false;

	float zDiff = destination.Z - position.Z;
	float zDiffAbs = FGenericPlatformMath::Abs(zDiff);
	
	bool slowDown = zDiffAbs <= speed.slowDownDistance;
	bool speedUp = !slowDown;

	if (slowDown)
	{
		speed.Min();
	}
	if(speedUp)
	{
		speed.currentSpeed = speed.currentSpeed + speed.acceleration;
	}
	speed.currentSpeed *= (1.0f - speed.friction);
	speed.Max();

	float movement = speed.currentSpeed * deltaTime;

	bool willOvershoot = movement >= zDiffAbs;
	if (willOvershoot)
	{
		movement = zDiffAbs;
		isMovementDone = true;

		speed.Reset();
	}

	bool isMovingDown = zDiff < 0;
	if (isMovingDown)
	{
		movement = -movement;
	}

	outPosition.Z += movement;

	return isMovementDone;
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

void AElevator::AddChild(UElevatorChildComponent& child)
{
	m_children.Add(&child);
}

void AElevator::RemoveChild(UElevatorChildComponent& child)
{
	m_children.Remove(&child);
}