// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PlayerName.h"
#include "Country.h"
#include "Sport.h"
#include "SportPlayer.generated.h"

USTRUCT(BluePrintType) struct FSportPlayer
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite) FPlayerName playerName;
	UPROPERTY(EditAnywhere, BlueprintReadWrite) int age;
	UPROPERTY(EditAnywhere, BlueprintReadWrite) int length;
	UPROPERTY(EditAnywhere, BlueprintReadWrite) float weight;
	UPROPERTY(EditAnywhere, BlueprintReadWrite) float number;
	UPROPERTY(EditAnywhere, BlueprintReadWrite) TEnumAsByte<Country> nationality;
	UPROPERTY(EditAnywhere, BlueprintReadWrite) TEnumAsByte<Sport> sport;

	FSportPlayer();
	~FSportPlayer();
};
