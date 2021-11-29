// Fill out your copyright notice in the Description page of Project Settings.


#include "ElevatorChildComponent.h"
#include "../Core/CoreSystem.h"
#include "Elevator.h"
#include "../Debug/Debug.h"

// Sets default values for this component's properties
UElevatorChildComponent::UElevatorChildComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
}


// Called when the game starts
void UElevatorChildComponent::BeginPlay()
{
	Super::BeginPlay();

	owner = GetOwner();
	m_physicalPosition = owner->GetActorLocation();

	if (UCoreSystem::Exists())
	{
		AElevator* elevator = UCoreSystem::Get().GetElevator();

		if (elevator != nullptr)
		{
			OnElevatorSpawn(elevator);
		}

		UEventSystem* eventSystem = UCoreSystem::Get().GetEventSystem();
		check(eventSystem != nullptr);
		eventSystem->OnElevatorSpawn().AddUObject(this, &UElevatorChildComponent::OnElevatorSpawn);
	}
}


// Called every frame
void UElevatorChildComponent::TickComponent(float deltaTime, ELevelTick tickType, FActorComponentTickFunction* thisTickFunction)
{
	Super::TickComponent(deltaTime, tickType, thisTickFunction);

	if (tickType == ELevelTick::LEVELTICK_All)
	{
		FVector desiredWorldPosition = m_physicalPosition + m_worldOffset;
		FVector toMove = desiredWorldPosition - owner->GetActorLocation();

		owner->AddActorWorldOffset(toMove, true);
	}
}

FVector& UElevatorChildComponent::GetWorldOffset()
{
	return m_worldOffset;
}

void UElevatorChildComponent::OnElevatorSpawn(AElevator* elevator)
{
	elevator->AddChild(this);
	UDebug::Log("Elevator spawned");
}