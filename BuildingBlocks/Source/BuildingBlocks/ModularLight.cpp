// Fill out your copyright notice in the Description page of Project Settings.


#include "ModularLight.h"

AModularLight::AModularLight() : AModularObject()
{
}

void AModularLight::BeginPlay()
{
	Super::BeginPlay();

	SetupParameter(m_isLightEnabled, "Toggle Light");
}

void AModularLight::Tick(float deltaTime)
{
	Super::Tick(deltaTime);

	lightIsEnabled = m_isLightEnabled;
}
