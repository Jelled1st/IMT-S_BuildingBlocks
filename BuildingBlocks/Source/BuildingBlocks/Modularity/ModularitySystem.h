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

	const TArray<TWeakObjectPtr<AModularObject>>& GetRegisteredObjects();
	const TArray<TWeakObjectPtr<UModularityComponent>>& GetRegisteredComponents();

private:
	UPROPERTY();
	TArray<TWeakObjectPtr<AModularObject>> m_modularObjects;

	UPROPERTY();
	TArray<TWeakObjectPtr<UModularityComponent>> m_modularityComponents;
};
