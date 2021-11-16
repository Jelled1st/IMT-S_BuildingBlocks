// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Runtime/Online/HTTP/Public/Http.h"
#include "UObject/NoExportTypes.h"
#include "Formula1Api.generated.h"

UCLASS()
class BUILDINGBLOCKS_API UFormula1Api : public UObject
{
	GENERATED_BODY()

public:
	struct TeamData
	{
	public:
		FString name;
		FString nationality;
		float points;
		int wins;
	};

	UFormula1Api();
	void Init(FHttpModule& newHttpModule);

	void PullApiData(bool applyDataToSportHandler = false);

	void ImportDataToSportHandler();

private:
	void PullTeamsData(bool applyDataToSportHandler = false);
	void PullPlayerData();

	FHttpModule* m_httpModule;

	TArray<TeamData> m_teams;
};
