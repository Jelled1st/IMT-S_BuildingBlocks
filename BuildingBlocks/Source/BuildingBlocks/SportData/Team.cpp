// Fill out your copyright notice in the Description page of Project Settings.


#include "Team.h"
#include "SportDataHandler.h"
#include "../Core/CoreSystem.h"
#include "../Debug/Debug.h"

UTeam::UTeam()
{
}

UTeam::~UTeam()
{
	UDebug::Log(*FString::Printf(TEXT("Destructing team %s"), *this->teamName));
}

void UTeam::AddPlayer(USportPlayer& player)
{
	players.Add(&player);
}

Sport UTeam::GetSport()
{
	return sport;
}

const FString& UTeam::GetName() const
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

FString UTeam::GetNationalityAsString()
{
	return nationalityAsString;
}

const TArray<USportPlayer*>& UTeam::GetPlayers()
{
	return players;
}

UTeam& UTeam::Make(FString teamName, Sport sport, float score, FString nationalityString, Country nationality)
{
	UTeam* team = NewObject<UTeam>();
	
	team->Init(teamName, sport, score, nationalityString, nationality);

	return *team;
}

bool UTeam::Init(FString newTeamName, Sport newSport, float newScore, FString nationalityString, Country newNationality)
{
	if (isInitialized)
	{
		isInitialized = false;
		UCoreSystem::Get().GetSportDataHandler().UnregisterTeam(*this);
	}

	this->teamName = newTeamName;
	this->sport = newSport;
	this->score = newScore;
	this->nationalityAsString = nationalityString;
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
