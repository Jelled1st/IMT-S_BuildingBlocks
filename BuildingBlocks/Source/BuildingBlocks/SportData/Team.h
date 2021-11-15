// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Country.h"
#include "Sport.h"
#include "SportPlayer.h"
#include "Team.generated.h"

UCLASS(BluePrintType) 
class BUILDINGBLOCKS_API UTeam : public UObject 
{
	GENERATED_BODY()

public:
	UTeam();
	~UTeam();

	static UTeam& Make(FString teamName, Sport sport, float score = 0, Country nationality = Country::Unknown);
	bool Init(FString newTeamName, Sport newSport, float newScore = 0, Country newNationality = Country::Unknown);

	Sport GetSport();
	FString GetName();
	float GetScore();
	Country GetNationality();

	UPROPERTY() bool isInitialized = false;

private:
	UPROPERTY() FString teamName;
	UPROPERTY() TArray<USportPlayer*> players;
	UPROPERTY() float score;
	UPROPERTY() FString currentChampionship;
	UPROPERTY() FString teamBoss;
	UPROPERTY() TEnumAsByte<Country> nationality;
	UPROPERTY() TEnumAsByte<Sport> sport;
};
