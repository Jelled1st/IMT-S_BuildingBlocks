// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "../EventSystem/EventSystem.h"

#include "../Modularity/ModularitySystem.h"
#include "../Modularity/PresetHandler.h"
#include "../Debug/DebugWindow.h"

#include "../SportData/SportDataHandler.h"

#include "../APIs/Formula1Api.h"
#include "../HTTPTestObject.h"

#include "../Gameplay/Elevator.h"

#include "CoreSystem.generated.h"

UCLASS(Blueprintable)
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

	UFUNCTION(BlueprintCallable)
	UEventSystem* const GetEventSystem() const
	{
		if (m_eventSystem == nullptr)
		{
			UE_DEBUG_BREAK();
			UDebug::Warning("GetEventSystem is nullptr");
		}
		return m_eventSystem;
	}

	UModularitySystem* GetModularitySystem()
	{
		if (m_modularitySystem == nullptr)
		{
			UE_DEBUG_BREAK();
			UDebug::Warning("modularitySystem is nullptr");
		}
		return m_modularitySystem;
	};

	UPresetHandler* GetPresetHandler()
	{
		if (m_presetHandler == nullptr)
		{
			UE_DEBUG_BREAK();
			UDebug::Warning("PresetHandler is nullptr");
		}
		return m_presetHandler;
	}

	UFUNCTION(BlueprintCallable)
	USportDataHandler* GetBPSportDataHandler()
	{
		if (m_sportDataHandler == nullptr)
		{
			UE_DEBUG_BREAK();
		}
		return m_sportDataHandler;
	}


	USportDataHandler& GetSportDataHandler()
	{
		if (m_sportDataHandler == nullptr)
		{
			UE_DEBUG_BREAK();
		}
		return *m_sportDataHandler;
	}

	UFUNCTION(BlueprintCallable)
	UHTTPTestObject* GetHttpTestObject()
	{
		if (m_htttpTestObject == nullptr)
		{
			UE_DEBUG_BREAK();
			UDebug::Warning("Requested Http Test Actor which is nullptr");
		}
		return m_htttpTestObject;
	}

	UFormula1Api* const GetF1Api() const
	{
		if (m_f1Api == nullptr)
		{
			UE_DEBUG_BREAK();
			UDebug::Warning("Requested F1 Api which is nullptr");
		}
		return m_f1Api;
	}

	AElevator* const GetElevator() const
	{
		if (!m_elevator.IsValid())
		{
			UE_DEBUG_BREAK();
			UDebug::Warning("Requested Elevator, which is nullptr");
		}
		return m_elevator.Get();
	}

private:
	void OnElevatorSpawn(AElevator* elevator);

	static TUniquePtr<UCoreSystem> m_instance;

	UModularitySystem* m_modularitySystem;

	UDebugWindow* m_debugWindow;

	UEventSystem* m_eventSystem;

	USportDataHandler* m_sportDataHandler;

	UHTTPTestObject* m_htttpTestObject;

	UFormula1Api* m_f1Api;

	TWeakObjectPtr<AElevator> m_elevator;

	UPresetHandler* m_presetHandler;
};
