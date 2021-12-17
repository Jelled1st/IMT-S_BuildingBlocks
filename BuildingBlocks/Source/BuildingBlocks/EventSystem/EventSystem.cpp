// Fill out your copyright notice in the Description page of Project Settings.


#include "EventSystem.h"

UEventSystem::UEventSystem()
{
	apiDataLoadedDelegate = FApiDataLoadedDelegate();

	m_apiDataLoadedEvent.AddUObject(this, &UEventSystem::OnApiDataLoadedEventCallback);

	apiTeamDataLoadedDelegate= FApiTeamDataLoadedDelegate();
	m_apiTeamDataLoadedEvent.AddUObject(this, &UEventSystem::OnApiTeamDataLoadedEventCallback);

}

UEventSystem::~UEventSystem()
{
}
