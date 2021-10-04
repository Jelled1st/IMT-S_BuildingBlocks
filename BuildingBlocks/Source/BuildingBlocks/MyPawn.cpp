// Fill out your copyright notice in the Description page of Project Settings.


#include "MyPawn.h"
#include "Debug/Debug.h"

// Sets default values
AMyPawn::AMyPawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	AutoPossessPlayer = EAutoReceiveInput::Player0;
}

// Called when the game starts or when spawned
void AMyPawn::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AMyPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AMyPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	Debug::Log("ye, is called");

	PlayerInputComponent->BindAction("ModularObject_SwapNext", IE_Pressed, modularObject, &AModularObject::SwapMeshNext);
	PlayerInputComponent->BindAction("ModularObject_SwapPrevious", IE_Pressed, modularObject, &AModularObject::SwapMeshPrevious);
}