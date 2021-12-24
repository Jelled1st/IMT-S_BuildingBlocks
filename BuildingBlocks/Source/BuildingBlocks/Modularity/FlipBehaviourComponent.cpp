// Fill out your copyright notice in the Description page of Project Settings.


#include "FlipBehaviourComponent.h"
#include "../Core/CoreSystem.h"

void UFlipBehaviourComponent::BeginPlay()
{
	Super::BeginPlay();

	SetupParameter(m_doFlip, "DoFlip");
	SetupParameter(m_inverseRotation, "Inverse rotation");

	SetupParameter(flipSpeed, "Flip Speed");
	SetupParameter(flipRotation, "Flip Rotation");
}

void UFlipBehaviourComponent::TickComponent(float deltaTime, ELevelTick tickType, FActorComponentTickFunction* thisTickFunction)
{
	Super::TickComponent(deltaTime, tickType, thisTickFunction);

	if (m_inverseRotation)
	{
		flipSpeed = -flipSpeed;
		flipRotation = -flipRotation;
		m_inverseRotation = false;
	}

	if (m_doFlip)
	{
		if ((flipRotation > 0 && flipSpeed > 0) || (flipRotation < 0 && flipSpeed < 0))
		{
			m_isFlipping = true;
		}
		else
		{
			m_doFlip = false;
			UDebug::Warning("Cannot flip because flipRotation or flipSpeed is negative. Make both positive or negative");
			UDebug::ToScreen("Cannot flip because flipRotation or flipSpeed is negative. Make both positive or negative", FColor::Red);
		}
	}

	if (m_isFlipping)
	{
		bool startOfFlip = currentRotation == 0;
		if (startOfFlip)
		{
			if (UCoreSystem::Exists())
			{
				UEventSystem* eventSystem = UCoreSystem::Get().GetEventSystem();
				if (eventSystem != nullptr)
				{
					eventSystem->CallPanelFlipEvent(this);
				}
			}
		}

		bool shouldEnd = false;

		float rotateNow = flipSpeed * deltaTime;

		bool willOvershoot = currentRotation + rotateNow >= flipRotation;
		
		if (flipSpeed < 0)
		{
			willOvershoot = currentRotation + rotateNow <= flipRotation;
		}

		if (willOvershoot)
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