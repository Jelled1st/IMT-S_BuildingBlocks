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

// Called when the game starts or when spawned
void UModularitySystem::Start()
{
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
