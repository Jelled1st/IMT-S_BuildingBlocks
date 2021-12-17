// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "../Modularity/ModularObject.h"
#include "ElevatorSpeed.h"
#include "Elevator.generated.h"

class UElevatorChildComponent;

UCLASS()
class BUILDINGBLOCKS_API AElevator : public AModularObject
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
	TArray<TSoftObjectPtr<AActor>> floorsSoftPtr;

	UPROPERTY(EditAnywhere);
	int startFloor = 0;

	UPROPERTY(EditAnywhere);
	TSoftObjectPtr<AActor> elevatorPlatformSoftPtr;

	UPROPERTY(EditAnywhere);
	FElevatorSpeed speed;

	void AddChild(UElevatorChildComponent& child);
	void RemoveChild(UElevatorChildComponent& child);

private:
	// Returns if movement is done
	bool Move(const FVector& position, FVector& outPosition, const FVector& destination, float deltaTime);

	void ConvertSoftPtrs();

	int m_currentFloorIndex;

	ElevatorState m_state = ElevatorState::Idle;
	int m_desinationIndex = 0;

	FVector m_startPosition;

	UPROPERTY();
	AActor* m_elevatorPlatform;

	UPROPERTY();
	TArray<AActor*> m_floors;

	UPROPERTY();
	TArray<UElevatorChildComponent*> m_children;

	bool m_moveUp;
	bool m_moveDown;
};
