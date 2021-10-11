// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

class BUILDINGBLOCKS_API EventSystem
{
public:
	EventSystem();
	~EventSystem();

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
