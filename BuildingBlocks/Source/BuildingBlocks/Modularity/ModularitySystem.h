// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ModularObject.h"
#include "ModularityComponent.h"

#include "ModularitySystem.generated.h"

UCLASS()
class BUILDINGBLOCKS_API UModularitySystem : public UObject
{	
	GENERATED_BODY()

public:	
	// Sets default values for this actor's properties
	UModularitySystem();
	~UModularitySystem();

public:	
	void RegisterObject(AModularObject& object);
	void UnregisterObject(AModularObject& object);

	void RegisterComponent(UModularityComponent& object);
	void UnregisterComponent(UModularityComponent& object);

	TArray<AModularObject*>& GetRegisteredObjects();
	TArray<UModularityComponent*>& GetRegisteredComponents();

private:
	UPROPERTY();
	TArray<AModularObject*> m_modularObjects;

	UPROPERTY();
	TArray<UModularityComponent*> m_modularityComponents;
};
