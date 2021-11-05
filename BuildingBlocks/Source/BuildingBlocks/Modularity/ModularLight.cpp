// Fill out your copyright notice in the Description page of Project Settings.


#include "ModularLight.h"

AModularLight::AModularLight() : AModularObject()
{
	spotLight = CreateDefaultSubobject<USpotLightComponent>("Spotlight");
	spotLight->SetupAttachment(RootComponent);
}

void AModularLight::BeginPlay()
{
	Super::BeginPlay();

	SetupParameter(m_isLightEnabled, "Toggle Light");
	SetupParameter(m_lightIntensity, "Light Intensity");
}

void AModularLight::Tick(float deltaTime)
{
	Super::Tick(deltaTime);

	spotLight->SetVisibility(m_isLightEnabled, true);
	spotLight->SetIntensity(static_cast<float>(m_lightIntensity));
}
