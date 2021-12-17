// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ImGuiCommon.h"
#include "../Utilities/Utility.h"

#include "../SportData/SportDataHandler.h"
#include "../SportData/Team.h"
#include "../SportData/SportPlayer.h"
#include "../SportData/Country.h"
#include "../SportData/Sport.h"

#include <string>

#if WITH_IMGUI
#include "..\..\..\Plugins\ImGui\Source\ImGui\Public\ImGuiDelegates.h"
class FImGuiModuleProperties;
class AModularObject;
class UModularityComponent;
#endif

#include "DebugWindow.generated.h"

#define OPERATOR_WINDOW false

UCLASS()
class BUILDINGBLOCKS_API UDebugWindow : public UObject
{
	GENERATED_BODY()

public:
	static bool debugWindowEnabled;

	struct TeamData
	{
	public:
		Sport sport;
		static const int nameLength = 21;
		char teamName[21] = { 0 };
		char nationality[21] = "Unknown";
		float score = 0;
	};

	struct PlayerData
	{
	public:
		Sport sport;
		static const int nameLength = 21;
		char firstName[21] = { 0 };
		char lastName[21] = { 0 };
		char displayName[21] = { 0 };
		char nationality[21] = "Unknown";
		float driverScoreF1 = 0;
		int number;
		UTeam* team;

		int dob_day = 1;
		int dob_month = 1;
		int dob_year = 1950;
	};

	UDebugWindow();
	~UDebugWindow();

	void Start();
	void Shutdown();

#if WITH_IMGUI
	void DrawWindow();

	enum TableDrawOption
	{
		None,
		SeperateFirst,
		SeperateAll,
	};

	static void ImGuiSliderVector(const char* label, FVector& vector, float xLimit = 1000, float yLimit = 1000, float zLimit = 1000);
#endif

private:
	float m_slideValue = 0;
	bool m_isSaveWindowOpen = false;
	bool nationality = false;
	bool teamName = false;
	bool numberOfPoints = false;
	bool numberOfDrivers = false;
	bool f1OptionNumberA = false;
	bool f1OptionNumberB = false;

#if WITH_IMGUI
	void DrawOperatorControls();
	void DrawObjectControls(AModularObject& object);
	void DrawComponentControls(UModularityComponent& object);

	void DrawElevatorControls();
	
	void DrawSportDatabase();
	void DrawCreateTeamMenu();
	void DrawCreatePlayerMenu(USportDataHandler& sportData);
	void DrawSportData(USportDataHandler& sportData, Sport sport);
	void DrawPlayersTable(USportDataHandler& sportData, const TArray<USportPlayer*>& players, Sport sport);

	void DrawPresetMenu();
	FString m_jsonString = "";
	bool m_deserializeSucceeded = true;
	AModularObject* m_selectedObject = nullptr;
	UModularityComponent* m_selectedComponent = nullptr;
	TeamData m_newTeam;
	PlayerData m_newPlayer;
	UTeam* m_selectedTeam = nullptr;
	USportPlayer* m_selectedPlayer = nullptr;

	static const int presetNameLength;
	char m_presetName[21] = { 0 };

	int m_rangeMin = -1000;
	int m_rangeMax = 1000;

	enum F1TeamData {
		TeamName,
		TeamScore,
		Nationality
	};

	TArray<F1TeamData> m_f1TeamData;

	static const char* m_sportsList[21];
	static const char* m_dataOptions[21];

	USportDataHandler::PlayerSort m_currentPlayerSort = USportDataHandler::PlayerSort::Team;

#if OPERATOR_WINDOW
	TSharedPtr<SWindow> m_window;
	TSharedPtr<SViewport> m_viewport;
#endif

#endif
};
