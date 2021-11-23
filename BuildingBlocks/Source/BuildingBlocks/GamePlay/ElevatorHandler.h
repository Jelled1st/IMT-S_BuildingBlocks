// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "ElevatorHandler.generated.h"

class AElevator;

UCLASS()
class BUILDINGBLOCKS_API UElevatorHandler : public UObject
{
	GENERATED_BODY()
	
public:
	void Register(AElevator& elevator);
	void Unregister(AElevator& elevator);

private:
	UPROPERTY();
	TArray<AElevator*> m_elevators;
};
