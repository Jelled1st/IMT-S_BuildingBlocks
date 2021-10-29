// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PlayerName.h"
#include "PlayerData.generated.h"

USTRUCT(BluePrintType) struct FPlayerData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite) FPlayerName playerName;
	UPROPERTY(EditAnywhere, BlueprintReadWrite) int length;
	UPROPERTY(EditAnywhere, BlueprintReadWrite) float weight;

	FPlayerData();
	~FPlayerData();
};
