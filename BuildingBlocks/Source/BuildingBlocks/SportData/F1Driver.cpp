// Fill out your copyright notice in the Description page of Project Settings.


#include "F1Driver.h"

UF1Driver::UF1Driver() : USportPlayer()
{
}

UF1Driver& UF1Driver::Make(FString firstName, FString lastName, FString displayName, UTeam& team)
{
	UF1Driver* driver = NewObject<UF1Driver>();

	driver->Init(firstName, lastName, displayName, team);

	return *driver;
}

bool UF1Driver::Init(FString firstName, FString lastName, FString displayName, UTeam& team)
{
	return Super::Init(firstName, lastName, displayName, team);
}