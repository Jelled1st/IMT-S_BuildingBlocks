// Fill out your copyright notice in the Description page of Project Settings.


#include "ModularitySystem.h"
#include "CoreSystem.h"

// Sets default values
AModularitySystem::AModularitySystem()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AModularitySystem::BeginPlay()
{
	Super::BeginPlay();

	selectedObject = nullptr;
	UCoreSystem::Get().SetModularitySystem(*this);
}

// Called every frame
void AModularitySystem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

