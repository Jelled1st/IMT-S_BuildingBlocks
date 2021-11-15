// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ImGuiCommon.h"
#include "../Utilities/Utility.h"

#include "../SportData/SportDataHandler.h"
#include "../SportData/Team.h"
#include "../SportData/SportPlayer.h"
#include "../SportData/PlayerName.h"
#include "../SportData/Country.h"
#include "../SportData/Sport.h"

#include <string>

#if WITH_IMGUI
#include "..\..\..\Plugins\ImGui\Source\ImGui\Public\ImGuiDelegates.h"
class FImGuiModuleProperties;
class AModularObject;
#endif

#include "DebugWindow.generated.h"

UCLASS()
class BUILDINGBLOCKS_API UDebugWindow : public UObject
{
	GENERATED_BODY()

public:
	struct TeamData
	{
	public:
		Sport sport;
		const int nameLength = 21;
		char teamName[21] = { 0 };
		Country selectedNationality = Country::Unknown;
		float score = 0;
	};

	struct PlayerData
	{
	public:
		const int nameLength = 21;
		char firstName[21] = { 0 };
		char lastName[21] = { 0 };
	};

	UDebugWindow();
	~UDebugWindow();

	void Start();

#if WITH_IMGUI
	void DrawWindow();

	static void ImGuiSliderVector(const char* label, FVector& vector, float xLimit = 1000, float yLimit = 1000, float zLimit = 1000);
#endif

private:
	float m_slideValue = 0;
	bool m_isSaveWindowOpen = false;

#if WITH_IMGUI
	void DrawOperatorControls();
	void DrawObjectControls(AModularObject& object);
	void DrawObjectTransform(AModularObject& object);
	
	void DrawSportDatabase();
	void DrawCreateTeamMenu();
	void DrawSportData(USportDataHandler& sportData, Sport sport);
	
	void Debug_GetF1TeamsFromAPI();

	void DrawPresetMenu();
	FString m_jsonString = "";
	bool m_deserializeSucceeded = true;
	AModularObject* m_selectedObject = nullptr;
	TeamData m_newTeam;
	UTeam* m_selectedTeam = nullptr;
#endif
};
