// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "ElevatorSpeed.generated.h"

USTRUCT(BlueprintType)
struct BUILDINGBLOCKS_API FElevatorSpeed
{
	GENERATED_BODY()
	
public:
	bool Max()
	{
		if (currentSpeed > maxSpeed)
		{
			currentSpeed = maxSpeed;
			return true;
		}
		return false;
	}

	bool Min()
	{
		if (currentSpeed < slowedMinSpeed)
		{
			currentSpeed = slowedMinSpeed;
			return true;
		}
		return false;
	}

	void Reset()
	{
		currentSpeed = 0.0f;
	}

	UPROPERTY(EditAnywhere);
	double acceleration = 20.0f;

	UPROPERTY(EditAnywhere);
	double maxSpeed = 250.0f;

	UPROPERTY(EditAnywhere);
	double slowedMinSpeed = 20.0f;

	UPROPERTY(EditAnywhere);
	double slowDownDistance = 125.0f;

	UPROPERTY(EditAnywhere);
	double friction = 0.1f;

	UPROPERTY(EditAnywhere);
	double currentSpeed = 0.0f;
};
