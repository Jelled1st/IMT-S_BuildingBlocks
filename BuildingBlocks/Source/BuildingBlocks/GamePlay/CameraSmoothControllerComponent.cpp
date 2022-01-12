// Fill out your copyright notice in the Description page of Project Settings.


#include "CameraSmoothControllerComponent.h"
#include "../Core/CoreSystem.h"
#include "../Debug/Debug.h"
#include "../Modularity/ModularityComponent.h"

UCameraSmoothControllerComponent::UCameraSmoothControllerComponent() : Super()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UCameraSmoothControllerComponent::TickComponent(float deltaTime, ELevelTick tickType, FActorComponentTickFunction* thisTickFunction)
{
	if (!UCoreSystem::Exists())
	{
		return;
	}

	APlayerController* controller = UCoreSystem::Get().GetPlayerController();

	if (controller->IsInputKeyDown(FKey("A")))
	{
		m_velocity.Y -= acceleration;
	}
	if (controller->IsInputKeyDown(FKey("D")))
	{
		m_velocity.Y += acceleration;
	}
	if (controller->IsInputKeyDown(FKey("W")))
	{
		m_velocity.X += acceleration;
	}
	if (controller->IsInputKeyDown(FKey("S")))
	{
		m_velocity.X -= acceleration;
	}

	float mouseX, mouseY;
	if (controller->GetMousePosition(mouseX, mouseY) && controller->IsInputKeyDown(FKey("E")))
	{
		FVector2D currentMouse = FVector2D(mouseX, mouseY);
		FVector2D mouseDiff = m_previousMouse - currentMouse;

		if (enableMouseX)
		{
			m_angularVelocity.Z -= mouseDiff.X * turnAcceleration;
		}
		if (enableMouseY)
		{
			m_angularVelocity.Y += mouseDiff.Y * turnAcceleration;
		}
	}
	m_previousMouse.Set(mouseX, mouseY);

	m_worldOffset += m_velocity * deltaTime;
	m_velocity *= ((100-friction)/100.0);

	if (modularityComponent != nullptr)
	{
		modularityComponent->rotX += m_angularVelocity.X * deltaTime;
		modularityComponent->rotY += m_angularVelocity.Y * deltaTime;
		modularityComponent->rotZ += m_angularVelocity.Z * deltaTime;
	}
	m_angularVelocity *= ((100 - turnFriction) / 100.0);

	Super::TickComponent(deltaTime, tickType, thisTickFunction);
}

void UCameraSmoothControllerComponent::BeginPlay()
{
	Super::BeginPlay();

	if (modularityComponent != nullptr)
	{
		modularityComponent->SetupParameter(acceleration, "Camera.Acceleration");
		modularityComponent->SetupParameter(friction, "Camera.Friction(0-100)");
		modularityComponent->SetupParameter(turnAcceleration, "Camera.TurnAcceleration");
		modularityComponent->SetupParameter(turnFriction, "Camera.TurnFriction(0-100)");
		modularityComponent->SetupParameter(enableMouseX, "Camera.enabledMouseX");
		modularityComponent->SetupParameter(enableMouseY, "Camera.enabledMouseY");
	}
}