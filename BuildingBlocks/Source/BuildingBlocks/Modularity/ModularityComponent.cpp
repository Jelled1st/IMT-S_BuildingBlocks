// Fill out your copyright notice in the Description page of Project Settings.


#include "ModularityComponent.h"
#include "../Debug/Debug.h"
#include "../Core/CoreSystem.h"

UModularityComponent::UModularityComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UModularityComponent::BeginPlay()
{
	Super::BeginPlay();
	
	m_owner = GetOwner();

	if (visualComponent != nullptr)
	{
		m_isEnabled = true;
	}
	else
	{
		TArray<UStaticMeshComponent*> components;
		m_owner->GetComponents<UStaticMeshComponent>(components);

		if (components.Num() == 1)
		{
			visualComponent = components[0];
			m_isEnabled = true;
		}
		else if (components.Num() > 1)
		{
			visualComponent = components[0];
			m_isEnabled = true;

			UDebug::ToScreen(*FString::Printf(TEXT("Warning: '%s' contains multiple static mesh components (using the first) - please assign one to the '%s' component"), *m_owner->GetName(), *this->GetName()), FColor::Red);
			UDebug::Warning(*FString::Printf(TEXT("'%s' contains multiple static mesh components (using the first) - please assign one to the '%s' component"), *m_owner->GetName(), *this->GetName()));
		}
		else if (components.Num() <= 0)
		{
			m_isEnabled = false;

			UDebug::ToScreen(*FString::Printf(TEXT("Warning: '%s' does not contain static mesh components - please assign one to the '%s' component"), *m_owner->GetName(), *this->GetName()), FColor::Red);
			UDebug::Warning(*FString::Printf(TEXT("'%s' does not contain static mesh components - please assign one to the '%s' component"), *m_owner->GetName(), *this->GetName()));
		}
	}

	if (!m_isEnabled)
	{
		return;
	}

	posX = m_owner->GetActorLocation().X;
	posY = m_owner->GetActorLocation().Y;
	posZ = m_owner->GetActorLocation().Z;

	rotX = m_owner->GetActorRotation().Euler().X;
	rotY = m_owner->GetActorRotation().Euler().Y;
	rotZ = m_owner->GetActorRotation().Euler().Z;

	scaleX = m_owner->GetActorScale().X;
	scaleY = m_owner->GetActorScale().Y;
	scaleZ = m_owner->GetActorScale().Z;

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

	UCoreSystem::Get().GetModularitySystem()->RegisterComponent(*this);
}

void UModularityComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (m_isEnabled)
	{
		if (m_owner->GetActorLocation() != m_previousPos)
		{
			FVector pos = m_owner->GetActorLocation();
			posX = pos.X;
			posY = pos.Y;
			posZ = pos.Z;
		}

		if (m_owner->GetActorRotation().Euler() != m_previousRot)
		{
			FVector rot = m_owner->GetActorRotation().Euler();
			rotX = rot.X;
			rotY = rot.Y;
			rotZ = rot.Z;
		}

		if (m_owner->GetActorScale() != m_previousScale)
		{
			FVector scale = m_owner->GetActorScale();
			scaleX = scale.X;
			scaleY = scale.Y;
			scaleZ = scale.Z;
		}

		FVector newPos = FVector(posX, posY, posZ);
		FVector newRot = FVector(rotX, rotY, rotZ);
		FVector newScale = FVector(scaleX, scaleY, scaleZ);

		m_owner->SetActorLocation(newPos);
		m_owner->SetActorRotation(FQuat::MakeFromEuler(newRot));
		m_owner->SetActorScale3D(newScale);

		m_previousPos = newPos;
		m_previousRot = newRot;
		m_previousScale = newScale;
	}
}

FString UModularityComponent::GetActorName() const
{
	return m_owner->GetName();
}

void UModularityComponent::SwapMeshNext()
{
	int newIndex = (m_currentMeshIndex + 1) % meshAssets.Num();
	if (SwapMesh(newIndex))
	{
		m_currentMeshIndex = newIndex;
	}
}

void UModularityComponent::SwapMeshPrevious()
{
	int newIndex = (m_currentMeshIndex + (meshAssets.Num() - 1)) % meshAssets.Num();
	if (SwapMesh(newIndex))
	{
		m_currentMeshIndex = newIndex;
	}
}

bool UModularityComponent::SwapMesh(int index)
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

void UModularityComponent::SwapMatNext()
{
	int newIndex = (m_currentMatIndex + 1) % materialAssets.Num();
	if (SwapMat(newIndex))
	{
		m_currentMatIndex = newIndex;
	}
}

void UModularityComponent::SwapMatPrevious()
{
	int newIndex = (m_currentMatIndex + (materialAssets.Num() - 1)) % materialAssets.Num();
	if (SwapMat(newIndex))
	{
		m_currentMatIndex = newIndex;
	}
}

bool UModularityComponent::SwapMat(int index)
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

UStaticMesh& UModularityComponent::GetMesh()
{
	return *visualComponent->GetStaticMesh();
}

bool UModularityComponent::TrySetMeshByName(const FString& meshName)
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

UMaterialInterface& UModularityComponent::GetMaterial()
{
	return *visualComponent->GetMaterial(0);
}

bool UModularityComponent::TrySetMaterialByName(const FString& materialName)
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

void UModularityComponent::SetupParameter(bool& value, FString name)
{
	SetupParameter(static_cast<void*>(&value), name, ExposableParameterType::Bool);
}

void UModularityComponent::SetupParameter(FString& value, FString name)
{
	SetupParameter(static_cast<void*>(&value), name, ExposableParameterType::String);
}

void UModularityComponent::SetupParameter(double& value, FString name)
{
	SetupParameter(static_cast<void*>(&value), name, ExposableParameterType::Double);
}

void UModularityComponent::SetupParameter(void* value, FString name, ExposableParameterType type)
{
	m_parameters.Add(name, TPair<ExposableParameterType, void*>(type, value));
}

TMap<FString, TPair<ExposableParameterType, void*>>& UModularityComponent::GetParameters()
{
	return m_parameters;
}

bool UModularityComponent::RemoveParameter(const FString& name)
{
	if (m_parameters.Contains(name))
	{
		m_parameters.Remove(name);
		return false;
	}
	else
	{
		return false;
	}
}