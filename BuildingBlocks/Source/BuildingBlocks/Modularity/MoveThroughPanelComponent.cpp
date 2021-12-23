// Fill out your copyright notice in the Description page of Project Settings.


#include "MoveThroughPanelComponent.h"
#include "../Core/CoreSystem.h"
#include "../EventSystem/EventSystem.h"


void UMoveThroughPanelComponent::BeginPlay()
{
	Super::BeginPlay();

	if (UCoreSystem::Exists())
	{
		UEventSystem* eventSystem = UCoreSystem::Get().GetEventSystem();

		eventSystem->OnPanelFlip().AddUObject(this, &UMoveThroughPanelComponent::OnPanelFlip);
	}

	if (panelSoftPtr.IsValid())
	{
		AActor* panel = panelSoftPtr.Get();

		m_flipBehaviour = panel->FindComponentByClass<UFlipBehaviourComponent>();
	}
}


void UMoveThroughPanelComponent::OnPanelFlip(UFlipBehaviourComponent* flipBehaviour)
{
	if (flipBehaviour == m_flipBehaviour)
	{
		m_isMoving = true;
		float flipTime = flipBehaviour->flipRotation / flipBehaviour->flipSpeed;
		m_speed = size / flipTime;
		m_currentMovement = 0;
	}
}

void UMoveThroughPanelComponent::TickComponent(float deltaTime, ELevelTick tickType, FActorComponentTickFunction* thisTickFunction)
{
	Super::TickComponent(deltaTime, tickType, thisTickFunction);

	if (m_isMoving)
	{
		float addToMove = m_speed * deltaTime;

		bool isDone = false;
		if (m_currentMovement + addToMove >= size)
		{
			isDone = true;
			addToMove = size - m_currentMovement;
		}
		m_currentMovement += addToMove;

		posZ += addToMove;

		if (isDone)
		{
			m_isMoving = false;
			m_currentMovement = 0;
		}
	}
}