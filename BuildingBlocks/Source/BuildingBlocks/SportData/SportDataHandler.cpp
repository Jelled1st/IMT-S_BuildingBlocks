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

const TArray<UTeam*>& USportDataHandler::GetCricketTeams() const
{
	return m_cricketTeams;
}

const TArray<UTeam*>& USportDataHandler::GetFootballTeams() const
{
	return m_footballTeams;
}

const TArray<UTeam*>& USportDataHandler::GetF1Teams() const
{
	return m_f1Teams;
}

const TArray<UTeam*>& USportDataHandler::GetTeams(Sport sport) const
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

const TArray<USportPlayer*> USportDataHandler::GetPlayers(Sport sport) const
{
	const TArray<UTeam*>& teams = GetTeams(sport);

	TArray<USportPlayer*> allPlayers;

	for (UTeam* team : teams)
	{
		const TArray<USportPlayer*>& players = team->GetPlayers();

		for (USportPlayer* player : players)
		{
			allPlayers.Add(player);
		}
	}

	return allPlayers;
}

const TArray<USportPlayer*> USportDataHandler::GetPlayersSorted(Sport sport, PlayerSort sorting) const
{
	const TArray<UTeam*>& teams = GetTeams(sport);

	TArray<USportPlayer*> allPlayers;

	for (UTeam* team : teams)
	{
		const TArray<USportPlayer*>& players = team->GetPlayers();

		for (USportPlayer* player : players)
		{
			allPlayers.Add(player);
		}
	}

	auto nrSort = [](const USportPlayer& a, const USportPlayer& b)
	{
		return a.GetNumber() < b.GetNumber();
	};

	auto scoreSort = [](const USportPlayer& a, const USportPlayer& b)
	{
		const UF1Driver* driverA = static_cast<const UF1Driver*>(&a);
		const UF1Driver* driverB = static_cast<const UF1Driver*>(&b);

		return driverA->championshipPoints > driverB->championshipPoints;
	};

	switch (sorting)
	{
	case USportDataHandler::Number:
		allPlayers.Sort(nrSort);
		break;

	case USportDataHandler::Score:
		if (sport == Sport::Formula1)
		{
			allPlayers.Sort(scoreSort);
		}
		break;
	}

	return allPlayers;
}

UTeam* USportDataHandler::FindTeam(FString name, Sport sport) const
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

TArray<UTeam*> USportDataHandler::GetAllTeams() const
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

float USportDataHandler::GetHighestScoreInSport(Sport sport) const
{
	const TArray<UTeam*>& teams = GetTeams(sport);

	float highestScore = 0;

	for (UTeam* team : teams)
	{
		if (team->score > highestScore)
		{
			highestScore = team->score;
		}
	}

	return highestScore;
}

float USportDataHandler::GetHighestDriverScoreF1() const
{
	const TArray<UTeam*>& teams = GetF1Teams();

	float highestScore = 0;

	for (UTeam* team : teams)
	{
		const TArray<USportPlayer*>& players = team->GetPlayers();

		for (USportPlayer* player : players)
		{
			UF1Driver* driver = static_cast<UF1Driver*>(player);

			if (driver->championshipPoints > highestScore)
			{
				highestScore = driver->championshipPoints;
			}
		}
	}

	return highestScore;
}

void USportDataHandler::ClearData(Sport sport)
{
	if (sport == Sport::Cricket)
	{
		m_cricketTeams.Empty();
	}
	else if (sport == Sport::Football)
	{
		m_footballTeams.Empty();
	}
	else //if(sport == Sport::Formula1)
	{
		m_f1Teams.Empty();
	}
}