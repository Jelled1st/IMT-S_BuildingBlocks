// Fill out your copyright notice in the Description page of Project Settings.


#include "Formula1ApiThreadHelper.h"
#include "../Debug/Debug.h"

FFormula1ApiThreadHelper::FFormula1ApiThreadHelper(UFormula1Api& f1Api) : FRunnable(), m_f1Api(&f1Api)
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

	bool isPulling = true;

	while (isPulling)
	{
		m_isRunning = true;
		bool wasSuccesfull = false;

		m_f1Api->PullConstructorsData();
		while (!m_f1Api->IsContructorDataPulled(wasSuccesfull)) {};

		wasSuccesfull = false;
		m_f1Api->PullDriverInformation();
		while (!m_f1Api->IsDriversInfoPulled(wasSuccesfull)) {};

		wasSuccesfull = false;
		m_f1Api->PullTeamDrivers();
		while (!m_f1Api->IsTeamDriversPulled(wasSuccesfull)) {};

		isPulling = false;
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