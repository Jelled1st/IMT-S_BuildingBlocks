// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ModularObject.h"

class BUILDINGBLOCKS_API AModularitySystem
{	
public:	
	// Sets default values for this actor's properties
	AModularitySystem();

public:	
	void Start();
	
	void RegisterObject(AModularObject& object);
	void UnregisterObject(AModularObject& object);

private:
	TArray<TSharedPtr<AModularObject>> m_modularObjects;
};
