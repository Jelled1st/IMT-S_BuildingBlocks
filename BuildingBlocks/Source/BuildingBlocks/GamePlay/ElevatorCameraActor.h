// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Camera/CameraActor.h"
#include "ElevatorCameraActor.generated.h"

UCLASS()
class BUILDINGBLOCKS_API AElevatorCameraActor : public ACameraActor, public FTickableGameObject
{
	GENERATED_BODY()

public:
	AElevatorCameraActor();

	void BeginPlay() override;
	void EndPlay(EEndPlayReason::Type endPlayReason) override;

	virtual void Tick(float deltaTime) override;
	virtual bool IsTickable() const override;
	virtual TStatId GetStatId() const override;

	FVector& GetWorldOffset();

private:
	FVector m_physicalPosition;
	FVector m_worldOffset;

	bool m_isCreatedOnRunning = false;
	bool m_endPlay = false;
};
