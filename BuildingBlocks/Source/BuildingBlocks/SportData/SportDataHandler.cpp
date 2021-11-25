// Fill out your copyright notice in the Description page of Project Settings.


#include "SportDataHandler.h"
#include "../Core/CoreSystem.h"
#include "../EventSystem/EventSystem.h"
#include "Sport.h"

USportDataHandler::USportDataHandler()
{
	m_cricketTeams = TArray<UTeam*>(nullptr, 0);
	m_footballTeams = TArray<UTeam*>(nullptr, 0);
	m_f1Teams = TArray<UTeam*>(nullptr, 0);

	if (UCoreSystem::Exists())
	{
		UEventSystem* const eventSystem = UCoreSystem::Get().GetEventSystem();
		if (eventSystem != nullptr)
		{
			eventSystem->OnApiDataLoaded().AddUObject(this, &USportDataHandler::OnSportDataLoaded);
		}
	}
}

USportDataHandler::~USportDataHandler()
{

}

bool USportDataHandler::RegisterTeam(UTeam& team)
{
	switch (team.GetSport())
	{
		case Sport::Cricket:
		{
			m_cricketTeams.Add(&team);
			return true;
		}
		case Sport::Football:
		{
			m_footballTeams.Add(&team);
			return true;
		}
		case Sport::Formula1:
		{
			m_f1Teams.Add(&team);
			return true;
		}
	}
	return false;
}

bool USportDataHandler::UnregisterTeam(UTeam& team)
{
	switch (team.GetSport())
	{
	case Sport::Cricket:
	{
		m_cricketTeams.Remove(&team);
		return true;
	}
	case Sport::Football:
	{
		m_footballTeams.Remove(&team);
		return true;
	}
	case Sport::Formula1:
	{
		m_f1Teams.Remove(&team);
		return true;
	}
	}
	return false;
}

const TArray<UTeam*>& USportDataHandler::GetCricketTeams()
{
	return m_cricketTeams;
}

const TArray<UTeam*>& USportDataHandler::GetFootballTeams()
{
	return m_footballTeams;
}

const TArray<UTeam*>& USportDataHandler::GetF1Teams()
{
	return m_f1Teams;
}

const TArray<UTeam*>& USportDataHandler::GetTeams(Sport sport)
{
	if (sport == Sport::Cricket)
	{
		return m_cricketTeams;
	}
	else if (sport == Sport::Football)
	{
		return m_footballTeams;
	}
	else //if(sport == Sport::Formula1)
	{
		return m_f1Teams;
	}
}

UTeam* USportDataHandler::FindTeam(FString name, Sport sport)
{
	const TArray<UTeam*>& teams = GetTeams(sport);

	for (UTeam* team : teams)
	{
		if (team->GetName() == name)
		{
			return team;
		}
	}
	return nullptr;
}

TArray<UTeam*> USportDataHandler::GetAllTeams()
{
	TArray<UTeam*> allTeams;
	for (UTeam* team : m_cricketTeams)
	{
		allTeams.Add(team);
	}
	for (UTeam* team : m_footballTeams)
	{
		allTeams.Add(team);
	}
	for (UTeam* team : m_f1Teams)
	{
		allTeams.Add(team);
	}
	return allTeams;
}

void USportDataHandler::OnSportDataLoaded(Sport sport)
{
	if (sport == Sport::Formula1)
	{
		if (UCoreSystem::Exists())
		{
			UFormula1Api* f1Api = UCoreSystem::Get().GetF1Api();

			if (f1Api != nullptr)
			{
				f1Api->ImportDataToSportHandler();
			}
		}
	}
}