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

	TArray<TSharedPtr<UTeam>> GetTeams(Sport sport);

private:
	TArray<TSharedPtr<UTeam>> m_cricketTeams;
	TArray<TSharedPtr<UTeam>> m_footballTeams;
	TArray<TSharedPtr<UTeam>> m_f1Teams;
};
