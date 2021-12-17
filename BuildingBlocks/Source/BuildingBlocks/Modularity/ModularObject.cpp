// Fill out your copyright notice in the Description page of Project Settings.


#include "ModularObject.h"
#include "../Debug/Debug.h"
#include "../Core/CoreSystem.h"
#include "ModularitySystem.h"

AModularObject::AModularObject()
{
	PrimaryActorTick.bCanEverTick = true;

	RootComponent = CreateDefaultSubobject<USceneComponent>("RootComponent");

	visualComponent = CreateDefaultSubobject<UStaticMeshComponent>("Visual Asset");
	visualComponent->SetupAttachment(RootComponent);
}

AModularObject::~AModularObject()
{
}

void AModularObject::BeginDestroy()
{
	Super::BeginDestroy();

	if (UCoreSystem::Exists())
	{
		UModularitySystem* system = UCoreSystem::Get().GetModularitySystem();
		if (system != nullptr)
		{
			system->UnregisterObject(*this);
		}
	}
}

void AModularObject::BeginPlay()
{
	Super::BeginPlay();

	posX = GetActorLocation().X;
	posY = GetActorLocation().Y;
	posZ = GetActorLocation().Z;

	rotX = GetActorRotation().Euler().X;
	rotY = GetActorRotation().Euler().Y;
	rotZ = GetActorRotation().Euler().Z;

	scaleX = GetActorScale().X;
	scaleY = GetActorScale().Y;
	scaleZ = GetActorScale().Z;

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
	
	SetupParameter(posX, "position.X");
	SetupParameter(posY, "position.Y");
	SetupParameter(posZ, "position.Z");

	SetupParameter(rotX, "rotation.X");
	SetupParameter(rotY, "rotation.Y");
	SetupParameter(rotZ, "rotation.Z");

	SetupParameter(scaleX, "scale.X");
	SetupParameter(scaleY, "scale.Y");
	SetupParameter(scaleZ, "scale.Z");

	UCoreSystem::Get().GetModularitySystem()->RegisterObject(*this);
}

void AModularObject::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	SetActorLocation(FVector(posX, posY, posZ));
	SetActorRotation(FQuat::MakeFromEuler(FVector(rotX, rotY, rotZ)));
	SetActorScale3D(FVector(scaleX, scaleY, scaleZ));
}

void AModularObject::SwapMeshNext()
{
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

UStaticMesh& AModularObject::GetMesh()
{
	return *visualComponent->GetStaticMesh();
}

bool AModularObject::TrySetMeshByName(const FString& meshName)
{
	int index = 0;
	for (UStaticMesh* mesh : meshAssets)
	{
		if (mesh->GetName() == meshName)
		{
			return SwapMesh(index);
		}

		++index;
	}

	return false;
}

UMaterialInterface& AModularObject::GetMaterial()
{
	return *visualComponent->GetMaterial(0);
}

bool AModularObject::TrySetMaterialByName(const FString& materialName)
{
	int index = 0;
	for (UMaterialInterface* material : materialAssets)
	{
		if (material->GetName() == materialName)
		{
			return SwapMat(index);
		}

		++index;
	}

	return false;
}

void AModularObject::SetupParameter(bool& value, FString name)
{
	SetupParameter(static_cast<void*>(&value), name, ExposableParameterType::Bool);
}

void AModularObject::SetupParameter(FString& value, FString name)
{
	SetupParameter(static_cast<void*>(&value), name, ExposableParameterType::String);
}

void AModularObject::SetupParameter(double& value, FString name)
{
	SetupParameter(static_cast<void*>(&value), name, ExposableParameterType::Double);
}

void AModularObject::SetupParameter(void* value, FString name, ExposableParameterType type)
{
	m_parameters.Add(name, TPair<ExposableParameterType, void*>(type, value));
}

TMap<FString, TPair<ExposableParameterType, void*>>& AModularObject::GetParameters()
{
	return m_parameters;
}