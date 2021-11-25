// Fill out your copyright notice in the Description page of Project Settings.


#include "ElevatorCameraActor.h"

AElevatorCameraActor::AElevatorCameraActor()
{
	PrimaryActorTick.bCanEverTick = true;

	m_isCreatedOnRunning = GIsRunning;
}

void AElevatorCameraActor::BeginPlay()
{
	Super::BeginPlay();

	m_physicalPosition = GetActorLocation();

	m_endPlay = false;
}

void AElevatorCameraActor::EndPlay(EEndPlayReason::Type endPlayReason)
{
	Super::EndPlay(endPlayReason);

	m_endPlay = true;
	SetActorLocation(m_physicalPosition);
}

void AElevatorCameraActor::Tick(float deltaTime)
{
	if (!m_endPlay)
	{
		SetActorLocation(m_physicalPosition + m_worldOffset);
	}
}

bool AElevatorCameraActor::IsTickable() const
{
	return m_isCreatedOnRunning;
}

TStatId AElevatorCameraActor::GetStatId() const
{
	return AActor::GetStatID();
}

FVector& AElevatorCameraActor::GetWorldOffset()
{
	return m_worldOffset;
}