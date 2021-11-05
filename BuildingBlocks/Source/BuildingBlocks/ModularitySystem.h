// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ModularObject.h"

#include "ModularitySystem.generated.h" 

UCLASS()
class BUILDINGBLOCKS_API UModularitySystem : public UObject
{	
	GENERATED_BODY()

public:	
	// Sets default values for this actor's properties
	UModularitySystem();

public:	
	void Start();
	
	void RegisterObject(AModularObject& object);
	void UnregisterObject(AModularObject& object);

	TArray<TSharedPtr<AModularObject>>& GetRegisteredObjects();

private:
	TArray<TSharedPtr<AModularObject>> m_modularObjects;
};
