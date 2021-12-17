// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Team.h"
#include "SportPlayer.h"
#include "F1Driver.h"
#include "SportDataHandler.generated.h"

UCLASS(BlueprintType)
class BUILDINGBLOCKS_API USportDataHandler : public UObject
{
	GENERATED_BODY()
	
public:
	enum PlayerSort
	{
		Number,
		Score,
		Team,
	};

	USportDataHandler();
	~USportDataHandler();

	bool RegisterTeam(UTeam& team);
	bool UnregisterTeam(UTeam& team);

	const TArray<UTeam*>& GetCricketTeams() const;
	const TArray<UTeam*>& GetFootballTeams() const;
	UFUNCTION(BlueprintCallable)
	const TArray<UTeam*>& GetF1Teams() const;

	const TArray<UTeam*>& GetTeams(Sport sport) const;
	TArray<UTeam*> GetAllTeams() const;

	const TArray<USportPlayer*> GetPlayers(Sport sport) const;
	const TArray<USportPlayer*> GetPlayersSorted(Sport sport, PlayerSort sorting) const;

	UTeam* FindTeam(FString name, Sport sport) const;

	void OnSportDataLoaded(Sport sport);

	float GetHighestScoreInSport(Sport sport) const;
	float GetHighestDriverScoreF1() const;

	void ClearData(Sport sport);

private:
	UPROPERTY();
	TArray<UTeam*> m_cricketTeams;

	UPROPERTY();
	TArray<UTeam*> m_footballTeams;

	UPROPERTY();
	TArray<UTeam*> m_f1Teams;
};
