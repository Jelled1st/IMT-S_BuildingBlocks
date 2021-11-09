// Fill out your copyright notice in the Description page of Project Settings.


#include "ModularObjectSaveSystem.h"



UModularObjectSaveSystem::UModularObjectSaveSystem() {
	//levelObjects= UCoreSystem::Get().GetModularitySystem()->GetRegisteredObjects();

}

void UModularObjectSaveSystem::SaveLevelObjects() {

	if (UModularObjectSaveSystem* SaveGameInstance = Cast<UModularObjectSaveSystem>(UGameplayStatics::CreateSaveGameObject(UModularObjectSaveSystem::StaticClass())))
	{

		// Set data on the savegame object.
		SaveGameInstance->m_levelObjects = UCoreSystem::Get().GetModularitySystem().GetRegisteredObjects();

		// Start async save process.
		UGameplayStatics::AsyncSaveGameToSlot(SaveGameInstance,"level1",0);
	}
}

TArray<AModularObject*> UModularObjectSaveSystem::returnSavedObjects()
{
	return m_levelObjects;
}

