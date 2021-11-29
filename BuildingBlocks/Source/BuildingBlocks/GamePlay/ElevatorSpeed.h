// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "ElevatorSpeed.generated.h"

UCLASS()
class BUILDINGBLOCKS_API UElevatorSpeed : public UObject
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere);
	float acceleration = 20.0f;

	UPROPERTY(EditAnywhere);
	float maxSpeed = 20.0f;
};
