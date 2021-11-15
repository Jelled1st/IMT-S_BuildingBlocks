// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerName.h"

UPlayerName::UPlayerName()
{
}

UPlayerName::~UPlayerName()
{
}

FString UPlayerName::ToString() const
{
	return FString::Printf(TEXT("%s - %s %s"), *m_displayName, *m_firstName, *m_lastName);
}
