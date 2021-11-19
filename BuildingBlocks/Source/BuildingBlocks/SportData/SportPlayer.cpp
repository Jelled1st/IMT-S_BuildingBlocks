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

	player->Init(firstName, lastName, displayName, team);

	return *player;
}

bool USportPlayer::Init(FString firstName, FString lastName, FString displayName, UTeam& team)
{
	this->m_firstName = firstName;
	this->m_lastName = lastName;
	this->m_displayName = displayName;
	this->m_team = &team;

	this->m_sport = team.GetSport();

	team.AddPlayer(*this);

	return true;
}

FString USportPlayer::GetFullName() const
{
	return FString::Printf(TEXT("%s %s"), *m_firstName, *m_lastName);
}

FString USportPlayer::GetDisplayName() const
{
	return m_displayName;
}

int USportPlayer::GetNumber() const
{
	return m_number;
}

FString USportPlayer::GetNumberAsString() const
{
	return FString::FromInt(m_number);
}