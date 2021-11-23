// Fill out your copyright notice in the Description page of Project Settings.

#include "Elevator.h"
#include "ElevatorHandler.h"

void UElevatorHandler::Register(AElevator& evelator)
{
	m_elevators.Add(&evelator);
}

void UElevatorHandler::Unregister(AElevator& evelator)
{
	m_elevators.Remove(&evelator);
}
