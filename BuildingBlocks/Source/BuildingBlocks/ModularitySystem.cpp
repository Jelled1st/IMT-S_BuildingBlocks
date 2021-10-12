// Fill out your copyright notice in the Description page of Project Settings.


#include "ModularitySystem.h"
#include "CoreSystem.h"

AModularitySystem::AModularitySystem()
{
}

// Called when the game starts or when spawned
void AModularitySystem::Start()
{
}

void AModularitySystem::RegisterObject(AModularObject& object)
{
	m_modularObjects.Add(TSharedPtr<AModularObject>(&object));
}

void AModularitySystem::UnregisterObject(AModularObject& object)
{
	m_modularObjects.Remove(TSharedPtr<AModularObject>(&object));
}
