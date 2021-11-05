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
	UPROPERTY(EditAnywhere, BlueprintReadWrite) FString teamName;
	UPROPERTY(EditAnywhere, BlueprintReadWrite) TArray<USportPlayer*> players;
	UPROPERTY(EditAnywhere, BlueprintReadWrite) float score;
	UPROPERTY(EditAnywhere, BlueprintReadWrite) FString currentChampionship;
	UPROPERTY(EditAnywhere, BlueprintReadWrite) FString teamBoss;
	UPROPERTY(EditAnywhere, BlueprintReadWrite) TEnumAsByte<Country> nationality;
	UPROPERTY(EditAnywhere, BlueprintReadWrite) TEnumAsByte<Sport> sport;

	UTeam();
	~UTeam();
};
