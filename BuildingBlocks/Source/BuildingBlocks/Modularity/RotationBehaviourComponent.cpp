// Fill out your copyright notice in the Description page of Project Settings.


#include "RotationBehaviourComponent.h"

void URotationBehaviourComponent::BeginPlay()
{
	Super::BeginPlay();

	SetupParameter(rotationSpeed, "RotationSpeed");
	SetupParameter(m_isRotating, "IsRotating");
}

void URotationBehaviourComponent::TickComponent(float deltaTime, ELevelTick tickType, FActorComponentTickFunction* thisTickFunction)
{
	Super::TickComponent(deltaTime, tickType, thisTickFunction);

	if (m_isRotating)
	{
		rotZ += rotationSpeed * deltaTime;
	}
}