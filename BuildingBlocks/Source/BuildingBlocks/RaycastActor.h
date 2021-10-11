// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DrawDebugHelpers.h"
#include "RaycastActor.generated.h"


UCLASS()
class BUILDINGBLOCKS_API ARaycastActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ARaycastActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	class ACharacter* player;
	UInputComponent* playerInputComponent;

private:
	void RayCast();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
