// Fill out your copyright notice in the Description page of Project Settings.


#include "SportDataHandler.h"
#include "Sport.h"

USportDataHandler::USportDataHandler()
{

}

USportDataHandler::~USportDataHandler()
{

}

bool USportDataHandler::RegisterTeam(UTeam& team)
{
	switch (team.sport)
	{
		case Sport::Cricket:
		{
			m_cricketTeams.Add(MakeShareable(&team));
			return true;
		}
		case Sport::Football:
		{
			m_footballTeams.Add(MakeShareable(&team));
			return true;
		}
		case Sport::Formula1:
		{
			m_f1Teams.Add(MakeShareable(&team));
			return true;
		}
	}
	return false;
}

TArray<TSharedPtr<UTeam>> USportDataHandler::GetTeams(Sport sport)
{
	switch (sport)
	{
		case Sport::Cricket:
		{
			return m_cricketTeams;
		}
		case Sport::Football:
		{
			return m_footballTeams;
		}
		case Sport::Formula1:
		{
			return m_f1Teams;
		}
	}
	return TArray<TSharedPtr<UTeam>>();
}
