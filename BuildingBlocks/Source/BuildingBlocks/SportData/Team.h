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
	UFUNCTION(BlueprintCallable)
	const FString& GetName() const;
	UFUNCTION(BlueprintCallable)
	float GetScore();
	Country GetNationality();
	UFUNCTION(BlueprintCallable)
	FString GetNationalityAsString();
	UFUNCTION(BlueprintCallable)
	const TArray<USportPlayer*>& GetPlayers();

	UPROPERTY() bool isInitialized = false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite) int wins;
	UPROPERTY(EditAnywhere, BlueprintReadWrite) float score;

private:
	FString teamName;
	UPROPERTY();
	TArray<USportPlayer*> players;
	FString currentChampionship;
	FString teamBoss;
	TEnumAsByte<Country> nationality;
	FString nationalityAsString;
	TEnumAsByte<Sport> sport;
};
