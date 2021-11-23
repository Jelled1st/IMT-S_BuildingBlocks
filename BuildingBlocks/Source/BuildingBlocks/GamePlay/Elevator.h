// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Elevator.generated.h"

UCLASS()
class BUILDINGBLOCKS_API AElevator : public AActor
{
	GENERATED_BODY()
	
public:	
	enum ElevatorState
	{
		Idle,
		Moving,
	};

	// Sets default values for this actor's properties
	AElevator();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float deltaTime) override;

	virtual void BeginDestroy() override;

	UFUNCTION(BlueprintCallable)
	void MoveUp(int floorCount);

	UFUNCTION(BlueprintCallable)
	void MoveDown(int floorCount);

	UPROPERTY(EditAnywhere);
	AActor* elevatorPlateau;

	UPROPERTY(EditAnywhere);
	TArray<AActor*> floors;

	UPROPERTY(EditAnywhere);
	TArray<AActor*> actors;

	UPROPERTY(EditAnywhere);
	float speed = 2.0f;

private:
	int m_currentFloorIndex;

	ElevatorState m_state = ElevatorState::Idle;
	int m_desinationIndex = 0;
};
