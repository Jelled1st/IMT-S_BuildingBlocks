// Fill out your copyright notice in the Description page of Project Settings.


#include "FlipBehaviourComponent.h"


void UFlipBehaviourComponent::BeginPlay()
{
	Super::BeginPlay();

	SetupParameter(m_doFlip, "DoFlip");
	SetupParameter(flipSpeed, "Flip Speed");
	SetupParameter(flipRotation, "Flip Rotation");
}

void UFlipBehaviourComponent::TickComponent(float deltaTime, ELevelTick tickType, FActorComponentTickFunction* thisTickFunction)
{
	Super::TickComponent(deltaTime, tickType, thisTickFunction);

	if (m_doFlip)
	{
		m_isFlipping = true;
	}

	if (m_isFlipping)
	{
		bool shouldEnd = false;

		float rotateNow = flipSpeed * deltaTime;

		if (currentRotation + rotateNow >= flipRotation)
		{
			rotateNow = flipRotation - currentRotation;

			shouldEnd = true;
		}
		currentRotation += rotateNow;


		rotX += rotateNow;

		if (shouldEnd)
		{
			m_isFlipping = false;
			m_doFlip = false;

			currentRotation = 0;
		}
	}
}