// Fill out your copyright notice in the Description page of Project Settings.


#include "CoreSystem.h"
#include "Debug/Debug.h"

TUniquePtr<UCoreSystem> UCoreSystem::m_instance;

UCoreSystem::UCoreSystem(const FObjectInitializer& objectInitializer) : Super(objectInitializer)
{
}

void UCoreSystem::Init()
{
	Super::Init();

	m_instance.Reset(this);
	Debug::Log("CoreSystem - Init");

	m_debugWindow = NewObject<UDebugWindow>();
	m_modularitySystem.Reset(new AModularitySystem());

	m_eventSystem = NewObject<UEventSystem>();
}

void UCoreSystem::OnStart()
{
	m_debugWindow->Start();
}

void UCoreSystem::Shutdown()
{
	m_debugWindow = nullptr;
	m_eventSystem = nullptr;

	m_modularitySystem.Release();
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
