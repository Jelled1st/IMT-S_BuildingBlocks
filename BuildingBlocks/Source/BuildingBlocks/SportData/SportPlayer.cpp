// Fill out your copyright notice in the Description page of Project Settings.


#include "SportPlayer.h"
#include "Team.h"

USportPlayer::USportPlayer()
{
}

USportPlayer::~USportPlayer()
{
}

USportPlayer& USportPlayer::Make(FString firstName, FString lastName, FString displayName, UTeam& team)
{
	USportPlayer* player = NewObject<USportPlayer>();

	player->m_firstName = firstName;
	player->m_lastName = lastName;
	player->m_displayName = displayName;
	player->m_team = &team;

	player->m_sport = team.GetSport();

	team.AddPlayer(*player);

	return *player;
}

FString USportPlayer::GetFullName() const
{
	return FString::Printf(TEXT("%s %s"), *m_firstName, *m_lastName);
}

FString USportPlayer::GetDisplayName() const
{
	return m_displayName;
}
