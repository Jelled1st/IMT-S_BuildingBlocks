// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Team.h"
#include "SportPlayer.h"
#include "PlayerName.h"
#include "SportDataHandler.generated.h"

UCLASS(BlueprintType)
class BUILDINGBLOCKS_API USportDataHandler : public UObject
{
	GENERATED_BODY()
	
public:
	USportDataHandler();
	~USportDataHandler();

	bool RegisterTeam(UTeam& team);

	const TArray<UTeam*>& GetCricketTeams();
	const TArray<UTeam*>& GetFootballTeams();
	const TArray<UTeam*>& GetF1Teams();

	const TArray<UTeam*>& GetTeams(Sport sport);
	TArray<UTeam*> GetAllTeams();

private:
	UPROPERTY();
	TArray<UTeam*> m_cricketTeams;

	UPROPERTY();
	TArray<UTeam*> m_footballTeams;

	UPROPERTY();
	TArray<UTeam*> m_f1Teams;
};
