// Fill out your copyright notice in the Description page of Project Settings.


#include "ModularObject.h"
#include "Debug/Debug.h"

AModularObject::AModularObject()
{
	PrimaryActorTick.bCanEverTick = true;

	RootComponent = CreateDefaultSubobject<USceneComponent>("RootComponent");

	visualComponent = CreateDefaultSubobject<UStaticMeshComponent>("Visual Asset");
	visualComponent->SetupAttachment(RootComponent);
}

void AModularObject::BeginPlay()
{
	Super::BeginPlay();

	if (meshAssets.Num() != 0)
	{
		m_currentMeshIndex = 0;
		visualComponent->SetStaticMesh(meshAssets[0]);
	}
	
}

void AModularObject::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AModularObject::SwapMeshNext()
{
	Debug::Log("");
	int newIndex = (m_currentMeshIndex + 1) % meshAssets.Num();
	if (SwapMesh(newIndex))
	{
		m_currentMeshIndex = newIndex;
	}
}

void AModularObject::SwapMeshPrevious()
{
	int newIndex = (m_currentMeshIndex + (meshAssets.Num()-1)) % meshAssets.Num();
	if (SwapMesh(newIndex))
	{
		m_currentMeshIndex = newIndex;
	}
}

bool AModularObject::SwapMesh(int index)
{
	if (index >= 0 || index < meshAssets.Num())
	{
		visualComponent->SetStaticMesh(meshAssets[index]);
		return true;
	}
	else
	{
		return false;
	}
}
