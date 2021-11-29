// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../SportData/Sport.h"
#include "../Gameplay/Elevator.h"
#include "EventSystem.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FApiDataLoadedDelegate, Sport, sport);

UCLASS(BlueprintType)
class BUILDINGBLOCKS_API UEventSystem : public UObject
{
	GENERATED_BODY()
public:
	UEventSystem();
	~UEventSystem();

	DECLARE_EVENT (EventSystem, TestEvent)
	TestEvent& OnTest()
	{
		return m_testEvent;
	}

	void CallTest()
	{
		m_testEvent.Broadcast();
	}

	DECLARE_EVENT_OneParam(EventSystem, ApiDataLoadedEvent, Sport)
	ApiDataLoadedEvent& OnApiDataLoaded()
	{
		return m_apiDataLoadedEvent;
	}

	void CallApiDataLoadedEvent(Sport sport)
	{
		m_apiDataLoadedEvent.Broadcast(sport);
	}

	UPROPERTY(BlueprintAssignable);
	FApiDataLoadedDelegate apiDataLoadedDelegate;

	void OnApiDataLoadedEventCallback(Sport sport)
	{
		apiDataLoadedDelegate.Broadcast(sport);
	}


	DECLARE_EVENT_OneParam(EventSystem, ElevatorSpawnedEvent, AElevator* const)
	ElevatorSpawnedEvent& OnElevatorSpawn()
	{
		return m_elevatorSpawnedEvent;
	}

	void CallElevatorSpawned(AElevator* const elevator)
	{
		m_elevatorSpawnedEvent.Broadcast(elevator);
	}

private:
	TestEvent m_testEvent;

	ApiDataLoadedEvent m_apiDataLoadedEvent;

	ElevatorSpawnedEvent m_elevatorSpawnedEvent;
};
