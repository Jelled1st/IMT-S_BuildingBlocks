// Fill out your copyright notice in the Description page of Project Settings.


#include "CoreSystem.h"

// Sets default values
ACoreSystem::ACoreSystem()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
}

// Called when the game starts or when spawned
void ACoreSystem::BeginPlay()
{
	Super::BeginPlay();
	
	if (!Exists())
	{
		m_core.Reset(this);
	}
}

// Called every frame
void ACoreSystem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

TUniquePtr<ACoreSystem> ACoreSystem::m_core;