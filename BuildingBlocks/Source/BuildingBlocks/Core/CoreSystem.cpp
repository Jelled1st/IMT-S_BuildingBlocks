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

	
	m_RowLength = m_sportDataHandler->GetF1Teams().Num();
}

void UCoreSystem::OnStart()
{
	m_debugWindow->Start();

	//get request example, can be called from wherever needed
	m_htttpTestObject->Start();

	m_f1Api->PullApiDataAsync();

}

void UCoreSystem::Shutdown()
{
	m_debugWindow->Shutdown();
	m_instance.Release();

	UDebug::Log("CoreSystem - Shutdown()");
}

UCoreSystem& UCoreSystem::Get()
{
	return *m_instance.Get();
}

bool UCoreSystem::Exists()
{
	return m_instance.IsValid();
}

void UCoreSystem::AddNationalityToArray()
{
	//TODO: too much duplicate code,remove code. Make team data aka (teams,score,names) as method parameter?
	m_ColumnLegnth++;
	for(UTeam *f1Team : m_sportDataHandler->GetF1Teams())
	{
		m_f1Data.Add(f1Team->GetNationalityAsString());

	}
	
}

void UCoreSystem::AddScoreToArray()
{
	m_ColumnLegnth++;
	for(UTeam * f1Team : m_sportDataHandler->GetF1Teams())
	{
		m_f1Data.Add(FString::SanitizeFloat(f1Team->GetScore()));

	}
}

void UCoreSystem::AddTeamNameToArray()
{
	m_ColumnLegnth++;
	for(UTeam * f1Team : m_sportDataHandler->GetF1Teams())
	{
		m_f1Data.Add(f1Team->GetName());

	}
}

void UCoreSystem::OnElevatorSpawn(AElevator* elevator)
{
	m_elevator = elevator;
}