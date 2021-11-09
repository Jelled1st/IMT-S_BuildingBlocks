// Fill out your copyright notice in the Description page of Project Settings.


#include "CoreSystem.h"
#include "../Debug/Debug.h"

TUniquePtr<UCoreSystem> UCoreSystem::m_instance;

UCoreSystem::UCoreSystem(const FObjectInitializer& objectInitializer) : Super(objectInitializer)
{
}

void UCoreSystem::Init()
{
	Super::Init();

	m_instance.Reset(this);
	UDebug::Log("CoreSystem - Init");

	m_debugWindow = NewObject<UDebugWindow>();
	m_modularitySystem = NewObject<UModularitySystem>();

	m_eventSystem = NewObject<UEventSystem>();
	m_sportDataHandler = NewObject<USportDataHandler>();
}

void UCoreSystem::OnStart()
{
	m_debugWindow->Start();
}

void UCoreSystem::Shutdown()
{
	m_sportDataHandler = nullptr;
	m_eventSystem = nullptr;
	m_modularitySystem = nullptr;
	m_debugWindow = nullptr;

	GEngine->ForceGarbageCollection();

	m_instance.Release();
}

UCoreSystem& UCoreSystem::Get()
{
	return *m_instance.Get();
}

bool UCoreSystem::Exists()
{
	return m_instance.IsValid();
}
