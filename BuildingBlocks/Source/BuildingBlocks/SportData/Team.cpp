// Fill out your copyright notice in the Description page of Project Settings.


#include "Team.h"
#include "SportDataHandler.h"
#include "../Core/CoreSystem.h"

UTeam::UTeam()
{
}

UTeam::~UTeam()
{
}


void UTeam::AddPlayer(USportPlayer& player)
{
	players.Add(&player);
}

Sport UTeam::GetSport()
{
	return sport;
}

FString UTeam::GetName()
{
	return teamName;
}

float UTeam::GetScore()
{
	return score;
}

Country UTeam::GetNationality()
{
	return nationality;
}

const TArray<USportPlayer*>& UTeam::GetPlayers()
{
	return players;
}

UTeam& UTeam::Make(FString teamName, Sport sport, float score, Country nationality)
{
	UTeam* team = NewObject<UTeam>();
	
	team->Init(teamName, sport, score, nationality);

	return *team;
}

bool UTeam::Init(FString newTeamName, Sport newSport, float newScore, Country newNationality)
{
	if (isInitialized)
	{
		isInitialized = false;
		UCoreSystem::Get().GetSportDataHandler().UnregisterTeam(*this);
	}

	this->teamName = newTeamName;
	this->sport = newSport;
	this->score = newScore;
	this->nationality = newNationality;

	if (UCoreSystem::Exists())
	{
		UCoreSystem::Get().GetSportDataHandler().RegisterTeam(*this);
		this->isInitialized = true;
		return true;
	}
	else
	{
		this->isInitialized = false;
		return false;
	}
}
