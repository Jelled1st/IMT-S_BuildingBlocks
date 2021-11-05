// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PlayerName.generated.h"

UCLASS(BluePrintType) 
class BUILDINGBLOCKS_API UPlayerName : public UObject
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite) FString firstName;
	UPROPERTY(EditAnywhere, BlueprintReadWrite) FString lastName;
	UPROPERTY(EditAnywhere, BlueprintReadWrite) FString displayName;

	UPlayerName();
	~UPlayerName();
};
