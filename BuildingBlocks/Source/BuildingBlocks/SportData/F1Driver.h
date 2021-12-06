// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SportPlayer.h"
#include "F1Driver.generated.h"

UCLASS(BluePrintType)
class BUILDINGBLOCKS_API UF1Driver : public USportPlayer
{
	GENERATED_BODY()
	
public:
	UF1Driver();

	static UF1Driver& Make(FString firstName, FString lastName, FString displayName, UTeam& team);
	bool Init(FString firstName, FString lastName, FString displayName, UTeam& team);

	UPROPERTY() float championshipPoints;
};
