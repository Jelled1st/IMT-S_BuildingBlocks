// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EventSystem.generated.h"

UCLASS()
class BUILDINGBLOCKS_API UEventSystem : public UObject
{
	GENERATED_BODY()
public:
	UEventSystem();
	~UEventSystem();

	DECLARE_EVENT (EventSystem, TestEvent)
	TestEvent& OnTest()
	{
		return m_testEvent;
	}

	void CallTest()
	{
		m_testEvent.Broadcast();
	}

private:
	TestEvent m_testEvent;
};
