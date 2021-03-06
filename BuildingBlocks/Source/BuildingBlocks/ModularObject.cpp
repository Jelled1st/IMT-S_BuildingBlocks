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
	if (materialAssets.Num() != 0)
	{
		m_currentMatIndex = 0;
		visualComponent->SetMaterial(0, materialAssets[0]);
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

void AModularObject::SwapMatNext()
{
	Debug::Log("");
	int newIndex = (m_currentMatIndex + 1) % materialAssets.Num();
	if (SwapMat(newIndex))
	{
		m_currentMatIndex = newIndex;
	}
}

void AModularObject::SwapMatPrevious()
{
	int newIndex = (m_currentMatIndex + (materialAssets.Num() - 1)) % materialAssets.Num();
	if (SwapMat(newIndex))
	{
		m_currentMatIndex = newIndex;
	}
}

bool AModularObject::SwapMat(int index)
{
	if (index >= 0 || index < materialAssets.Num())
	{
		visualComponent->SetMaterial(0, materialAssets[index]);
		return true;
	}
	else
	{
		return false;
	}
}

void AModularObject::Select()
{
	Debug::Log("Selected");
}
