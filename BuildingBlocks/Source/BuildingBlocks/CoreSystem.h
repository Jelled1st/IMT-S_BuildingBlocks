// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "EventSystem/EventSystem.h"

#include "ModularitySystem.h"
#include "Debug/DebugWindow.h"

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

	EventSystem eventSystem;

	AModularitySystem* GetModularitySystem()
	{
		return m_modularitySystem.Get();
	};

private:
	static TUniquePtr<UCoreSystem> m_instance;
	TUniquePtr<AModularitySystem> m_modularitySystem;
	TUniquePtr<DebugWindow> m_debugWindow;
};
