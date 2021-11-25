// Fill out your copyright notice in the Description page of Project Settings.

#include "ElevatorHandler.h"
#include "Elevator.h"

void UElevatorHandler::Register(AElevator& evelator)
{
	m_elevators.Add(&evelator);
}

void UElevatorHandler::Unregister(AElevator& evelator)
{
	m_elevators.Remove(&evelator);
}

const TArray<AElevator*>& UElevatorHandler::GetElevators() const
{
	return m_elevators;
}
