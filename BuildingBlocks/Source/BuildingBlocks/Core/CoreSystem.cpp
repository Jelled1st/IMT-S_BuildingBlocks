// Fill out your copyright notice in the Description page of Project Settings.


#include "CoreSystem.h"
#include "../Debug/Debug.h"
#include "Runtime/Online/HTTP/Public/Http.h"

TUniquePtr<UCoreSystem> UCoreSystem::m_instance;

UCoreSystem::UCoreSystem(const FObjectInitializer& objectInitializer) : Super(objectInitializer)
{
}

void UCoreSystem::Init()
{
	Super::Init();

	tickDelegateHandle = FTicker::GetCoreTicker().AddTicker(FTickerDelegate::CreateUObject(this, &UCoreSystem::Tick));

	m_instance.Reset(this);
	UDebug::Log("CoreSystem - Init");

	m_debugWindow = NewObject<UDebugWindow>();
	m_modularitySystem = NewObject<UModularitySystem>();
	m_presetHandler = NewObject<UPresetHandler>();

	m_eventSystem = NewObject<UEventSystem>();
	m_eventSystem->OnElevatorSpawn().AddUObject(this, &UCoreSystem::OnElevatorSpawn);

	m_sportDataHandler = NewObject<USportDataHandler>();

	m_htttpTestObject= NewObject<UHTTPTestObject>();

	m_f1Api = NewObject<UFormula1Api>();
	m_f1Api->Init(FHttpModule::Get());

}

void UCoreSystem::OnStart()
{
	Super::OnStart();

	m_debugWindow->Start();

	//get request example, can be called from wherever needed
	m_htttpTestObject->Start();

	m_f1Api->PullApiDataAsync();

}

void UCoreSystem::Shutdown()
{
	Super::Shutdown();

	FTicker::GetCoreTicker().RemoveTicker(tickDelegateHandle);

	m_debugWindow->Shutdown();
	m_instance.Release();

	UDebug::Log("CoreSystem - Shutdown()");
}

bool UCoreSystem::Tick(float deltaTime)
{
	if(this->GetWorld()->GetFirstPlayerController()->IsInputKeyDown(FKey("P")))
	{
		if (!debugTogglePressed)
		{
			m_debugWindow->isEnabled = !m_debugWindow->isEnabled;
		}
		debugTogglePressed = true;
	}
	else
	{
		debugTogglePressed = false;
	}

	return true;
}

UCoreSystem& UCoreSystem::Get()
{
	return *m_instance.Get();
}

bool UCoreSystem::Exists()
{
	return m_instance.IsValid();
}

void UCoreSystem::OnElevatorSpawn(AElevator* elevator)
{
	m_elevator = elevator;
}