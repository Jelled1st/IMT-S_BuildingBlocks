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

	m_debugWindow.Reset(new DebugWindow());
	m_modularitySystem.Reset(new AModularitySystem());
}

void UCoreSystem::OnStart()
{
	m_debugWindow.Get()->Start();
}

void UCoreSystem::Shutdown()
{
	m_debugWindow.Release();
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
