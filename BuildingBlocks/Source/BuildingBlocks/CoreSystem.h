// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CoreSystem.generated.h"

UCLASS()
class BUILDINGBLOCKS_API ACoreSystem : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACoreSystem();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	static ACoreSystem& GetCore()
	{
		return *m_core.Get();
	}

	static bool Exists()
	{
		return m_core.IsValid();
	}

	UPROPERTY(EditAnywhere);
	int x = 5;

private:

	static TUniquePtr<ACoreSystem> m_core;
};
