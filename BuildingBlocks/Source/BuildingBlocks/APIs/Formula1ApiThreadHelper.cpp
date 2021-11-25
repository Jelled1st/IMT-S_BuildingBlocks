// Fill out your copyright notice in the Description page of Project Settings.


#include "Formula1ApiThreadHelper.h"
#include "../Debug/Debug.h"
#include "../Core/CoreSystem.h"
#include "../EventSystem/EventSystem.h"

FFormula1ApiThreadHelper::FFormula1ApiThreadHelper(UFormula1Api& f1Api) : FRunnable(), m_f1Api(&f1Api)
{

}

FFormula1ApiThreadHelper::~FFormula1ApiThreadHelper()
{

}

bool FFormula1ApiThreadHelper::Init()
{
	UDebug::Log("Formula1ApiThreadHelper::Init()");
	m_isRunning = true;
	return true;
}

uint32 FFormula1ApiThreadHelper::Run()
{
	UDebug::Log("Formula1ApiThreadHelper::Run()");

	m_isRunning = true;
	bool wasSuccesfull = false;

	UDebug::Log("PullConstructorsData()");
	m_f1Api->PullConstructorsData();
	while (!m_f1Api->IsContructorDataPulled(wasSuccesfull)) {};

	UDebug::Log("PullDriverInformation()");
	wasSuccesfull = false;
	m_f1Api->PullDriverInformation();
	while (!m_f1Api->IsDriversInfoPulled(wasSuccesfull)) {};

	UDebug::Log("PullTeamDrivers()");
	wasSuccesfull = false;
	m_f1Api->PullTeamDrivers();
	while (!m_f1Api->IsTeamDriversPulled(wasSuccesfull)) {};

	UDebug::Log("pulling is done");

	if (UCoreSystem::Exists())
	{
		UEventSystem* const eventSystem = UCoreSystem::Get().GetEventSystem();
		if (eventSystem != nullptr)
		{
			eventSystem->CallApiDataLoadedEvent(Sport::Formula1);
		}
	}

	UDebug::Log("returning Formula1ApiThreadHelper::Run()");
	return 0;
}

void FFormula1ApiThreadHelper::Stop()
{
	UDebug::Log("Formula1ApiThreadHelper::Stop()");
	m_isRunning = false;
}

void FFormula1ApiThreadHelper::Exit()
{
	UDebug::Log("Formula1ApiThreadHelper::Exit()");
	m_isRunning = false;
}

bool FFormula1ApiThreadHelper::IsRunning()
{
	return m_isRunning;
}