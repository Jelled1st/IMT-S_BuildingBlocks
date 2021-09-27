// Fill out your copyright notice in the Description page of Project Settings.


#include "ImGuiRunner.h"

// Sets default values
AImGuiRunner::AImGuiRunner()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AImGuiRunner::BeginPlay()
{
	Super::BeginPlay();

	imGuiExample = new ImGuiExample();
}

// Called every frame
void AImGuiRunner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

