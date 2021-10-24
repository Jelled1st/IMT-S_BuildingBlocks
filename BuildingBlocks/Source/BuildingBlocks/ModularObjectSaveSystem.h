// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ModularObject.h"
#include "ModularitySystem.h"
#include "CoreSystem.h"
#include "GameFramework/SaveGame.h"
#include "Kismet/GameplayStatics.h"
#include "ModularObjectSaveSystem.generated.h"

/**
 * 
 */
UCLASS()
class BUILDINGBLOCKS_API UModularObjectSaveSystem : public USaveGame
{
	GENERATED_BODY()

public:

	UModularObjectSaveSystem();
	

	UFUNCTION(BlueprintCallable)
	void SaveLevelObjects();

	
	TArray<TSharedPtr<AModularObject>> returnSavedObjects();

	TArray<TSharedPtr<AModularObject>> m_levelObjects;

	
	
};
