// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "../EventSystem/EventSystem.h"

#include "../Modularity/ModularitySystem.h"
#include "../Debug/DebugWindow.h"

#include "../SportData/SportDataHandler.h"

#include "../APIs/Formula1Api.h"
#include "../HTTPTestObject.h"

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

	UFormula1Api* const GetF1Api()
	{
		if (m_f1Api == nullptr)
		{
			UE_DEBUG_BREAK();
			UDebug::Warning("Requested F1 Api which is nullptr");
		}
		return m_f1Api;
	}

private:
	static TUniquePtr<UCoreSystem> m_instance;

	UPROPERTY();
	UModularitySystem* m_modularitySystem;

	UPROPERTY();
	UDebugWindow* m_debugWindow;

	UPROPERTY();
	UEventSystem* m_eventSystem;

	UPROPERTY();
	USportDataHandler* m_sportDataHandler;

	UPROPERTY();
	UHTTPTestObject* m_htttpTestObject;

	UPROPERTY();
	UFormula1Api* m_f1Api;
	
};
