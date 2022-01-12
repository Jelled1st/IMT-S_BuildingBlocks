// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ElevatorChildComponent.h"
#include "CameraSmoothControllerComponent.generated.h"

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class BUILDINGBLOCKS_API UCameraSmoothControllerComponent : public UElevatorChildComponent
{
	GENERATED_BODY()

public:

	UCameraSmoothControllerComponent();

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UPROPERTY(EditAnywhere);
	double acceleration = 5.0;

	UPROPERTY(EditAnywhere);
	double friction = 20.0;

	UPROPERTY(EditAnywhere);
	double turnAcceleration = 5.0;

	UPROPERTY(EditAnywhere);
	double turnFriction = 20.0;

	UPROPERTY(EditAnywhere);
	bool enableMouseX = true;

	UPROPERTY(EditAnywhere);
	bool enableMouseY = true;

protected:
	virtual void BeginPlay() override;

	FVector m_velocity;
	FVector m_angularVelocity;
	FVector2D m_previousMouse;
};
