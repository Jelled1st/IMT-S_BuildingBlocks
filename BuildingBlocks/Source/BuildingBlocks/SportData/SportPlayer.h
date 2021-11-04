// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PlayerName.h"
#include "Country.h"
#include "SportPlayer.generated.h"

// Is called SportPlayer since player is already a class in UE4
USTRUCT(BluePrintType) struct FSportPlayer
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite) FPlayerName playerName;
	UPROPERTY(EditAnywhere, BlueprintReadWrite) int length;
	UPROPERTY(EditAnywhere, BlueprintReadWrite) float weight;
	UPROPERTY(EditAnywhere, BlueprintReadWrite) TEnumAsByte<Country> nationality;

	FSportPlayer();
	~FSportPlayer();
};
