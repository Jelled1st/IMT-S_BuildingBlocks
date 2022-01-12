// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/SceneComponent.h"
#include "../Modularity/ModularityComponent.h"
#include "ElevatorChildComponent.generated.h"

class AElevator;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BUILDINGBLOCKS_API UElevatorChildComponent : public USceneComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UElevatorChildComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	virtual void BeginDestroy() override;

	FVector& GetWorldOffset();

	UPROPERTY(EditAnywhere);
	AActor* owner;

protected:
	void OnElevatorSpawn(AElevator* elevator);

	FVector m_physicalPosition;
	FVector m_worldOffset;

	UPROPERTY();
	UModularityComponent* modularityComponent;
};
