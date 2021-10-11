// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ModularObject.h"
#include "ModularitySystem.generated.h"

UCLASS()
class BUILDINGBLOCKS_API AModularitySystem : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AModularitySystem();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable)
	void SelectObject(AModularObject* object)
	{
		selectedObject = object;
	}

	UFUNCTION(BlueprintCallable)
	void DeselectObject()
	{
		selectedObject = nullptr;
	}

	UFUNCTION(BlueprintCallable)
	AModularObject* GetSelected()
	{
		return selectedObject;
	}

private:
	AModularObject* selectedObject;
};
