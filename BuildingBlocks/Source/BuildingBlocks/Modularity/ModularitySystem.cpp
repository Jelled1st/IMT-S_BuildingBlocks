// Fill out your copyright notice in the Description page of Project Settings.


#include "ModularitySystem.h"
#include "../Core/CoreSystem.h"

UModularitySystem::UModularitySystem()
{
}

// Called when the game starts or when spawned
void UModularitySystem::Start()
{
}

void UModularitySystem::RegisterObject(AModularObject& object)
{
	m_modularObjects.Add(TSharedPtr<AModularObject>(&object));
}

void UModularitySystem::UnregisterObject(AModularObject& object)
{
	m_modularObjects.Remove(TSharedPtr<AModularObject>(&object));
}

TArray<TSharedPtr<AModularObject>>& UModularitySystem::GetRegisteredObjects()
{
	return m_modularObjects;
}
