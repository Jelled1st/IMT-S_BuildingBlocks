// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PlayerName.generated.h"

USTRUCT(BluePrintType) struct FPlayerName
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite) FString firstName;
	UPROPERTY(EditAnywhere, BlueprintReadWrite) FString lastName;
	UPROPERTY(EditAnywhere, BlueprintReadWrite) FString displayName;

	FPlayerName();
	~FPlayerName();
};
