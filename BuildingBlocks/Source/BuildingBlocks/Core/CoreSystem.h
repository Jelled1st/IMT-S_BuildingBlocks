// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "../EventSystem/EventSystem.h"

#include "../Modularity/ModularitySystem.h"
#include "../Debug/DebugWindow.h"

#include "../SportData/SportDataHandler.h"
#include "../HTTPTestObject.h"

#include "CoreSystem.generated.h"

UCLASS()
class BUILDINGBLOCKS_API UCoreSystem : public UGameInstance
{
	GENERATED_BODY()

public:
	UCoreSystem(const FObjectInitializer& objectInitializer);

	virtual void Init();
	virtual void OnStart();
	virtual void Shutdown();

	static UCoreSystem& Get();
	static bool Exists();

	UEventSystem& GetEventSystem()
	{
		if (m_eventSystem == nullptr)
		{
			UE_DEBUG_BREAK();
		}
		return *m_eventSystem;
	}

	UModularitySystem* GetModularitySystem()
	{
		return m_modularitySystem;
		{
			UE_DEBUG_BREAK();
		}
	};

	USportDataHandler& GetSportDataHandler()
	{
		if (m_sportDataHandler == nullptr)
		{
			UE_DEBUG_BREAK();
		}
		return *m_sportDataHandler;
	}

private:
	static TUniquePtr<UCoreSystem> m_instance;

	UModularitySystem* m_modularitySystem;
	UDebugWindow* m_debugWindow;
	UEventSystem* m_eventSystem;
	USportDataHandler* m_sportDataHandler;
	UHTTPTestObject* m_htttpTestObject;
	
};
