// Fill out your copyright notice in the Description page of Project Settings.


#include "Team.h"
#include "SportDataHandler.h"
#include "../Core/CoreSystem.h"

UTeam::UTeam()
{
	if (UCoreSystem::Exists())
	{
		UCoreSystem::Get().GetSportDataHandler().RegisterTeam(*this);
	}
}

UTeam::UTeam(FString teamName, Sport sport, float score, Country nationality) :
	teamName(teamName), sport(sport), score(score), nationality(nationality)
{
	UCoreSystem::Get().GetSportDataHandler().RegisterTeam(*this);
}

UTeam::~UTeam()
{
}
