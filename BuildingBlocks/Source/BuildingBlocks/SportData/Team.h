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

	static UTeam& Make(FString teamName, Sport sport, float score = 0, FString nationalityString = "Unknown", Country nationality = Country::Unknown);
	bool Init(FString newTeamName, Sport newSport, float newScore = 0, FString nationalityString = "Unknown", Country newNationality = Country::Unknown);

	void AddPlayer(USportPlayer& player);

	Sport GetSport();
	FString GetName();
	float GetScore();
	Country GetNationality();
	FString GetNationalityAsString();
	const TArray<USportPlayer*>& GetPlayers();

	UPROPERTY() bool isInitialized = false;

private:
	UPROPERTY() FString teamName;
	UPROPERTY() TArray<USportPlayer*> players;
	UPROPERTY() float score;
	UPROPERTY() FString currentChampionship;
	UPROPERTY() FString teamBoss;
	UPROPERTY() TEnumAsByte<Country> nationality;
	UPROPERTY() FString nationalityAsString;
	UPROPERTY() TEnumAsByte<Sport> sport;
};
