// Fill out your copyright notice in the Description page of Project Settings.


#include "ModularitySystem.h"
#include "../Core/CoreSystem.h"

UModularitySystem::UModularitySystem()
{
}

UModularitySystem::~UModularitySystem()
{
	m_modularObjects.Empty();
}

void UModularitySystem::RegisterObject(AModularObject& object)
{
	m_modularObjects.Add(&object);
}

void UModularitySystem::UnregisterObject(AModularObject& object)
{
	m_modularObjects.Remove(&object);
}

TArray<AModularObject*>& UModularitySystem::GetRegisteredObjects()
{
	return m_modularObjects;
}

void UModularitySystem::RegisterComponent(UModularityComponent& object)
{
	m_modularityComponents.Add(&object);
}

void UModularitySystem::UnregisterComponent(UModularityComponent& object)
{
	m_modularityComponents.Remove(&object);
}

TArray<UModularityComponent*>& UModularitySystem::GetRegisteredComponents()
{
	return m_modularityComponents;
}