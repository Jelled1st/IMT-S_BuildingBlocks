// Fill out your copyright notice in the Description page of Project Settings.


#include "SaveGameActor.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h" 
#include "Kismet/GameplayStatics.h"
#include "ModularObjectSaveSystem.h"
#include "ModularObject.h"
#include "ModularitySystem.h"
#include "Components/BoxComponent.h"
#include "GameFramework/Character.h"

// Sets default values
ASaveGameActor::ASaveGameActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ASaveGameActor::BeginPlay()
{
	Super::BeginPlay();


	if (UModularObjectSaveSystem* LoadedGame = Cast<UModularObjectSaveSystem>(UGameplayStatics::LoadGameFromSlot("level1", 0)))
	{
		//TO DO:check if save exists before loading, load appropiate objects based on level/scene
		UE_LOG(LogTemp, Warning, TEXT("LOADED GAME OBJS!"));

		TArray<TSharedPtr<AModularObject>> modularObjs = UCoreSystem::Get().GetModularitySystem().GetRegisteredObjects();


		for (int i = 0; i < modularObjs.Num(); i++) {
			//TODO: set all modular object meshes and materials based on the saved object properties



		}

	}
	else {
		UE_LOG(LogTemp, Warning, TEXT("COULDNT LOAD GAME OBJS!"));
	}

	
}

// Called every frame
void ASaveGameActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (UModularObjectSaveSystem* SaveGameInstance = Cast<UModularObjectSaveSystem>(UGameplayStatics::CreateSaveGameObject(UModularObjectSaveSystem::StaticClass())))
	{
		//TO DO: create delegate to save only when one object is changed
		SaveGameInstance->m_levelObjects = UCoreSystem::Get().GetModularitySystem().GetRegisteredObjects();

		UGameplayStatics::AsyncSaveGameToSlot(SaveGameInstance, "level1", 0);

		UE_LOG(LogTemp, Warning, TEXT("Some warning message"));
	}



}

