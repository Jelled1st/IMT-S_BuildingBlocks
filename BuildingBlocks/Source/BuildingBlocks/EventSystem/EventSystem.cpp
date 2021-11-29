// Fill out your copyright notice in the Description page of Project Settings.


#include "EventSystem.h"

UEventSystem::UEventSystem()
{
	apiDataLoadedDelegate = FApiDataLoadedDelegate();

	m_apiDataLoadedEvent.AddUObject(this, &UEventSystem::OnApiDataLoadedEventCallback);

}

UEventSystem::~UEventSystem()
{
}
