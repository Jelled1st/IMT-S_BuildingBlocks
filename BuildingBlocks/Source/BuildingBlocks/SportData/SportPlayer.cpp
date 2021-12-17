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

void USportPlayer::SetNumber(int number)
{
	m_number = number;
}

void USportPlayer::SetDateOfBirth(const FDateTime& dob)
{
	m_dateOfBirth = dob;
}

void USportPlayer::SetDateOfBirth(int day, int month, int year)
{
	m_dateOfBirth = FDateTime(year, month, day);
}

FString USportPlayer::GetFullName() const
{
	return m_firstName + " " + m_lastName;
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

FString USportPlayer::GetNationalityAsString() const
{
	return nationalityAsString;
}

const FDateTime& USportPlayer::GetDateOfBirth() const
{
	return m_dateOfBirth;
}


const UTeam* const USportPlayer::GetTeam() const
{
	return m_team;
}