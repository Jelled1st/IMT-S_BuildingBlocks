// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "EventSystem/EventSystem.h"

#include "ModularitySystem.h"

#include "CoreSystem.generated.h"

UCLASS()
class BUILDINGBLOCKS_API UCoreSystem : public UGameInstance
{
	GENERATED_BODY()

public:
	UCoreSystem(const FObjectInitializer& objectInitializer);

	virtual void Init();
	virtual void Shutdown();

	static UCoreSystem& Get();

	EventSystem eventSystem;

	AModularitySystem* GetModularitySystem()
	{
		return m_modularitySystem.Get();
	};

	void SetModularitySystem(AModularitySystem& system);

private:
	static TUniquePtr<UCoreSystem> m_instance;
	TUniquePtr<AModularitySystem> m_modularitySystem;
};
