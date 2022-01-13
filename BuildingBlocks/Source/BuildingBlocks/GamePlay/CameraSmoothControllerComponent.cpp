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

	friction = FGenericPlatformMath::Max(0.0, FGenericPlatformMath::Min(friction, 1000.0));
	turnFriction = FGenericPlatformMath::Max(0.0, FGenericPlatformMath::Min(turnFriction, 1000.0));

	APlayerController* controller = UCoreSystem::Get().GetPlayerController();

	FVector inputVelocity;

	const float accelerationFloat = static_cast<float>(acceleration);

	if (controller->IsInputKeyDown(FKey("A")))
	{
		inputVelocity.Y -= accelerationFloat;
	}
	if (controller->IsInputKeyDown(FKey("D")))
	{
		inputVelocity.Y += accelerationFloat;
	}
	if (controller->IsInputKeyDown(FKey("W")))
	{
		inputVelocity.X += accelerationFloat;
	}
	if (controller->IsInputKeyDown(FKey("S")))
	{
		inputVelocity.X -= accelerationFloat;
	}
	if (controller->IsInputKeyDown(FKey("SpaceBar")))
	{
		m_velocity.Z += accelerationFloat;
	}
	if (controller->IsInputKeyDown(FKey("LeftShift")))
	{
		m_velocity.Z -= accelerationFloat;
	}

	FQuat rotation = owner->GetActorRotation().Quaternion();
	FVector directionalAcceleration = rotation * inputVelocity;
	m_velocity += directionalAcceleration;

	m_worldOffset += m_velocity * deltaTime;
	m_velocity *= ((100 - friction) / 100.0);


	const float turnccelerationFloat = static_cast<float>(turnAcceleration);
	float mouseX, mouseY;
	if (controller->GetMousePosition(mouseX, mouseY) && controller->IsInputKeyDown(FKey("RightMouseButton")))
	{
		FVector2D currentMouse = FVector2D(mouseX, mouseY);
		FVector2D mouseDiff = m_previousMouse - currentMouse;

		if (enableMouseX)
		{
			m_angularVelocity.Z -= mouseDiff.X * turnccelerationFloat;
		}
		if (enableMouseY)
		{
			m_angularVelocity.Y += mouseDiff.Y * turnccelerationFloat;
		}
	}
	m_previousMouse.Set(mouseX, mouseY);

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