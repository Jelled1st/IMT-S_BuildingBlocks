// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Country.h"
#include "Sport.h"
#include "SportPlayer.h"
#include "Team.generated.h"

USTRUCT(BluePrintType) struct FTeam
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite) FString teamName;
	UPROPERTY(EditAnywhere, BlueprintReadWrite) TArray<FSportPlayer> players;
	UPROPERTY(EditAnywhere, BlueprintReadWrite) float score;
	UPROPERTY(EditAnywhere, BlueprintReadWrite) FString currentChampionship;
	UPROPERTY(EditAnywhere, BlueprintReadWrite) FString teamBoss;
	UPROPERTY(EditAnywhere, BlueprintReadWrite) TEnumAsByte<Country> nationality;
	UPROPERTY(EditAnywhere, BlueprintReadWrite) TEnumAsByte<Sport> sport;

	FTeam();
	~FTeam();
};
