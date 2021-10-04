// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ModularObject.generated.h"

UCLASS()
class BUILDINGBLOCKS_API AModularObject : public AActor
{
	GENERATED_BODY()
	
public:	
	AModularObject();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere);
	UStaticMeshComponent* visualComponent;

	UPROPERTY(EditAnywhere);
	TArray<UStaticMesh*> meshAssets;

	void SwapMeshNext();

	void SwapMeshPrevious();

	// Swaps the mesh of 
	bool SwapMesh(int index);

private:
	int m_currentMeshIndex = 0;
};
