// Fill out your copyright notice in the Description page of Project Settings.


#include "DebugWindow.h"

#include "../../../Plugins/ImGui/Source/ImGui/Private/ImGuiPrivatePCH.h"
#include "../../../Plugins/ImGui/Source/ImGui/Public/ImGuiModuleProperties.h"

#include "Debug.h"
#include "../Modularity/ModularObject.h"
#include "../Modularity/ModularityComponent.h"
#include "../Modularity/ModularitySystem.h"
#include "../Core/CoreSystem.h"
#include "Dom/JsonObject.h"
#include "Json.h"
#include "Containers/Map.h"
#include "../SportData/F1Driver.h"

#include "Misc/DateTime.h"
#include "../Gameplay/Elevator.h"

#include <string>

bool UDebugWindow::debugWindowEnabled = true;
#if WITH_IMGUI
const int UDebugWindow::presetNameLength = 21;
const char* UDebugWindow::m_sportsList[21] = { "Cricket", "Football", "Formula1" };
#endif

UDebugWindow::UDebugWindow()
{
}

UDebugWindow::~UDebugWindow()
{
}

void UDebugWindow::Start()
{
#if WITH_IMGUI

#if OPERATOR_WINDOW
	FSlateRenderer* renderer = FSlateApplication::Get().GetRenderer();

	m_window = SNew(SWindow)
		.AutoCenter(EAutoCenter::None)
		.Title(FText::FromString(TEXT("Control Window")))
		.IsInitiallyMaximized(false)
		.ScreenPosition(FVector2D(0, 0))
		.ClientSize(FVector2D(500, 800))
		.CreateTitleBar(true)
		.SizingRule(ESizingRule::UserSized)
		.SupportsMaximize(false)
		.SupportsMinimize(true)
		.HasCloseButton(true);

	TSharedRef<SWindow> windowRef = m_window.ToSharedRef();

	FSlateApplication& slateApp = FSlateApplication::Get();
	slateApp.AddWindow(windowRef, true);

	m_viewport = SNew(SViewport);
	windowRef->SetContent(m_viewport.ToSharedRef());
	m_viewport->SetForegroundColor(TAttribute<FSlateColor>());

#endif

	FImGuiDelegates::OnWorldDebug().AddLambda([this]() { DrawWindow(); });
#endif
}

void UDebugWindow::Shutdown()
{
#if OPERATOR_WINDOW
	m_window->RequestDestroyWindow();
	m_window.Reset();
#endif
}

#if WITH_IMGUI
void UDebugWindow::DrawWindow()
{
	if (!debugWindowEnabled)
	{
		return;
	}

	ImGui::Begin("Debug Window");

	if (ImGui::BeginTabBar("TabBar"))
	{
		if (ImGui::BeginTabItem("Operator Controls"))
		{
			DrawOperatorControls();
			ImGui::EndTabItem();
		}

		if (ImGui::BeginTabItem("Elevators"))
		{
			DrawElevatorControls();
			ImGui::EndTabItem();
		}

		if (ImGui::BeginTabItem("Sport Database"))
		{
			DrawSportDatabase();
			ImGui::EndTabItem();
		}

		if (ImGui::BeginTabItem("Example"))
		{
			ImGui::Text("Hello world!");

			if (ImGui::Button("Debug::Log('Hello World')"))
			{
				UDebug::Log("Hello World");
			}

			ImGui::SliderFloat("Slider", &m_slideValue, -5, 5);

			ImGui::EndTabItem();
		}

		ImGui::EndTabBar();
	}

	ImGui::End();
}

void UDebugWindow::DrawOperatorControls()
{
	static const int groupSize = 110;
	static const ImVec2 buttonSize = ImVec2(groupSize, 25);
	static const int verticalLineSize = 30;
	static const int verticalLineSpacing = 20;

	if (ImGui::TreeNode("Presets"))
	{
		DrawPresetMenu();
		ImGui::TreePop();
	}

	ImGui::Separator();

	ImGui::Text("Adjust the range of sliders. Use CTRL + click to enter manually");

	ImGui::SliderInt("Range min", &m_rangeMin, -1000, 0);
	ImGui::SliderInt("Range max", &m_rangeMax, 0, 1000);

	ImGui::Separator();

	ImGui::BeginGroup();

	TArray<AModularObject*>& modularObjs = UCoreSystem::Get().GetModularitySystem()->GetRegisteredObjects();

	AModularObject* currentSelected = m_selectedObject;

	if (modularObjs.Num() > 0)
	{
		for (AModularObject* obj : modularObjs)
		{
			if (obj == currentSelected)
			{
				ImGui::PushStyleColor(0, ImVec4(0, 1, 0, 1));
			}

			FString name = obj->GetName();
			if (ImGui::Button(TCHAR_TO_ANSI(*name), buttonSize))
			{
				m_selectedObject = obj;
				m_selectedComponent = nullptr;
			}

			if (obj == currentSelected)
			{
				ImGui::PopStyleColor();
			}
		}
	}
	else
	{
		m_selectedObject = nullptr;
		m_selectedComponent = nullptr;
	}

	TArray<UModularityComponent*>& modularComponents = UCoreSystem::Get().GetModularitySystem()->GetRegisteredComponents();

	UModularityComponent* currentSelectedComp = m_selectedComponent;

	if (modularObjs.Num() > 0)
	{
		for (UModularityComponent* comp : modularComponents)
		{
			if (comp == currentSelectedComp)
			{
				ImGui::PushStyleColor(0, ImVec4(0, 1, 0, 1));
			}

			FString name = comp->GetActorName();
			if (ImGui::Button(TCHAR_TO_ANSI(*name), buttonSize))
			{
				m_selectedComponent = comp;
				m_selectedObject = nullptr;
			}

			if (comp == currentSelectedComp)
			{
				ImGui::PopStyleColor();
			}
		}
	}
	else
	{
		m_selectedComponent = nullptr;
		m_selectedObject = nullptr;
	}

	ImGui::EndGroup();

	ImGui::SameLine(groupSize + verticalLineSpacing);

	ImGui::BeginGroup();

	for (int i = 0; i < verticalLineSize; ++i)
	{
		ImGui::Text("|");
	}

	ImGui::EndGroup();

	ImGui::SameLine(groupSize + verticalLineSpacing * 2);

	ImGui::BeginGroup();

	if (m_selectedObject != nullptr)
	{
		DrawObjectControls(*m_selectedObject);
	}

	if (m_selectedComponent != nullptr)
	{
		DrawComponentControls(*m_selectedComponent);
	}
	ImGui::EndGroup();
}

void UDebugWindow::DrawPresetMenu()
{
	UPresetHandler* handler = nullptr;

	if (UCoreSystem::Exists())
	{
		handler = UCoreSystem::Get().GetPresetHandler();

		if (handler == nullptr)
		{
			return;
		}
	}

	if (ImGui::Button("Save presets to file"))
	{
		handler->SavePresetsToFile();
	}

	ImGui::SameLine();

	if (ImGui::Button("Load presets from file"))
	{
		handler->LoadPresetsFromFile();
	}

	ImGui::NewLine();

	ImGui::Text("Presets");

	const static int presetsPerRow = 5;
	TArray<FString> presets = handler->GetPresetNames();

	int index = 0;
	for (FString preset : presets)
	{
		FString id = FString::Printf(TEXT("preset_%s"), *preset);
		ImGui::PushID(UUtility::FStringToCharPtr(*id));

		if (ImGui::Button(UUtility::FStringToCharPtr(*preset)))
		{
		}

		ImGui::PopID();

		bool isLastInRow = (index+1) % presetsPerRow == 0;
		bool isLast = index == (presets.Num() - 1);
		if (!isLastInRow && !isLast)
		{
			ImGui::SameLine();
		}
		++index;
	}

	ImGui::NewLine();

	ImGui::Text("Save new preset");

	ImGui::InputText("Preset Name", m_presetName, presetNameLength);

	if (ImGui::Button("Save preset"))
	{
		handler->SavePreset(UUtility::CharPtrToFString(m_presetName));
	}

	ImGui::SameLine();

	if (ImGui::Button("Load preset"))
	{
		handler->LoadPreset(UUtility::CharPtrToFString(m_presetName));
	}
}

void UDebugWindow::DrawObjectControls(AModularObject& object)
{
	ImGui::Text(TCHAR_TO_ANSI(*object.GetName()));

	static float arrowButtonSpacing = 40;
	static float textSpacing = 160;

	ImGui::PushID("MeshSelection");
	if (ImGui::Button("<"))
	{
		object.SwapMeshPrevious();
	}
	ImGui::SameLine(arrowButtonSpacing);

	ImGui::Text(TCHAR_TO_ANSI(*object.GetMesh().GetName()));

	ImGui::SameLine(arrowButtonSpacing + textSpacing);
	if (ImGui::Button(">"))
	{
		object.SwapMeshNext();
	}
	ImGui::PopID();


	ImGui::PushID("MaterialSelection");
	if (ImGui::Button("<"))
	{
		object.SwapMatPrevious();
	}
	ImGui::SameLine(arrowButtonSpacing);

	ImGui::Text(TCHAR_TO_ANSI(*object.GetMaterial().GetName()));

	ImGui::SameLine(arrowButtonSpacing + textSpacing);
	if (ImGui::Button(">"))
	{
		object.SwapMatNext();
	}
	ImGui::PopID();

	ImGui::NewLine();
	ImGui::Separator();
	ImGui::NewLine();

	TMap<FString, TPair<ExposableParameterType, void*>>& parameters = object.GetParameters();

	FString currentGrouping = "";

	for (TPair<FString, TPair<ExposableParameterType, void* >> parameter : parameters)
	{
		FString parameterName = parameter.Key;

		if (parameterName.Contains(TEXT(".")))
		{
			int seperatorIndex = parameterName.Find(TEXT("."));

			FString groupName = parameterName.Mid(0, seperatorIndex);
			parameterName = parameterName.Mid(seperatorIndex + 1, parameterName.Len() - seperatorIndex - 1);

			if (groupName != currentGrouping)
			{
				ImGui::NewLine();
				ImGui::Text(TCHAR_TO_ANSI(*groupName));
				ImGui::PushID(TCHAR_TO_ANSI(*groupName));
			}
			currentGrouping = groupName;
		}
		else
		{
			if (currentGrouping != "")
			{
				ImGui::PopID();
				ImGui::NewLine();
			}
			currentGrouping = "";
		}

		ExposableParameterType parameterType = parameter.Value.Key;

		switch (parameterType)
		{
		case ExposableParameterType::Bool:
		{
			ImGui::Checkbox(TCHAR_TO_ANSI(*parameterName), reinterpret_cast<bool*>(parameter.Value.Value));
			break;
		}
		case ExposableParameterType::String:
		{
			FString* stringPtr = reinterpret_cast<FString*>(parameter.Value.Value);
			char* stringAsChar = TCHAR_TO_ANSI(*(*stringPtr));

			ImGui::InputText(TCHAR_TO_ANSI(*parameterName), stringAsChar, 1000);

			FString outValue = FString(stringAsChar);

			*stringPtr = outValue;
			break;
		}
		case ExposableParameterType::Double:
		{
			double* doubleValue = reinterpret_cast<double*>(parameter.Value.Value);
			float floatValue = static_cast<float>(*doubleValue);

			ImGui::SliderFloat(TCHAR_TO_ANSI(*parameterName), &floatValue, m_rangeMin, m_rangeMax);

			*doubleValue = static_cast<double>(floatValue);

			break;
		}

		}
	}
	if (currentGrouping != "")
	{
		ImGui::PopID();
		ImGui::NewLine();
	}
}

void UDebugWindow::DrawComponentControls(UModularityComponent& component)
{
	ImGui::Text(TCHAR_TO_ANSI(*component.GetActorName()));

	static float arrowButtonSpacing = 40;
	static float textSpacing = 160;

	ImGui::PushID("MeshSelection");
	if (ImGui::Button("<"))
	{
		component.SwapMeshPrevious();
	}
	ImGui::SameLine(arrowButtonSpacing);

	ImGui::Text(TCHAR_TO_ANSI(*component.GetMesh().GetName()));

	ImGui::SameLine(arrowButtonSpacing + textSpacing);
	if (ImGui::Button(">"))
	{
		component.SwapMeshNext();
	}
	ImGui::PopID();


	ImGui::PushID("MaterialSelection");
	if (ImGui::Button("<"))
	{
		component.SwapMatPrevious();
	}
	ImGui::SameLine(arrowButtonSpacing);

	ImGui::Text(TCHAR_TO_ANSI(*component.GetMaterial().GetName()));

	ImGui::SameLine(arrowButtonSpacing + textSpacing);
	if (ImGui::Button(">"))
	{
		component.SwapMatNext();
	}
	ImGui::PopID();

	ImGui::NewLine();
	ImGui::Separator();
	ImGui::NewLine();

	TMap<FString, TPair<ExposableParameterType, void*>>& parameters = component.GetParameters();

	FString currentGrouping = "";

	for (TPair<FString, TPair<ExposableParameterType, void* >> parameter : parameters)
	{
		FString parameterName = parameter.Key;

		if (parameterName.Contains(TEXT(".")))
		{
			int seperatorIndex = parameterName.Find(TEXT("."));

			FString groupName = parameterName.Mid(0, seperatorIndex);
			parameterName = parameterName.Mid(seperatorIndex + 1, parameterName.Len() - seperatorIndex - 1);

			if (groupName != currentGrouping)
			{
				ImGui::NewLine();
				ImGui::Text(TCHAR_TO_ANSI(*groupName));
				ImGui::PushID(TCHAR_TO_ANSI(*groupName));
			}
			currentGrouping = groupName;
		}
		else
		{
			if (currentGrouping != "")
			{
				ImGui::PopID();
				ImGui::NewLine();
			}
			currentGrouping = "";
		}

		ExposableParameterType parameterType = parameter.Value.Key;

		switch (parameterType)
		{
		case ExposableParameterType::Bool:
		{
			ImGui::Checkbox(TCHAR_TO_ANSI(*parameterName), reinterpret_cast<bool*>(parameter.Value.Value));
			break;
		}
		case ExposableParameterType::String:
		{
			FString* stringPtr = reinterpret_cast<FString*>(parameter.Value.Value);
			char* stringAsChar = TCHAR_TO_ANSI(*(*stringPtr));

			ImGui::InputText(TCHAR_TO_ANSI(*parameterName), stringAsChar, 1000);

			FString outValue = FString(stringAsChar);

			*stringPtr = outValue;
			break;
		}
		case ExposableParameterType::Double:
		{
			double* doubleValue = reinterpret_cast<double*>(parameter.Value.Value);
			float floatValue = static_cast<float>(*doubleValue);

			ImGui::SliderFloat(TCHAR_TO_ANSI(*parameterName), &floatValue, m_rangeMin, m_rangeMax);

			*doubleValue = static_cast<double>(floatValue);

			break;
		}

		}
	}
	if (currentGrouping != "")
	{
		ImGui::PopID();
		ImGui::NewLine();
	}
}

void UDebugWindow::DrawElevatorControls()
{
	if (!UCoreSystem::Exists())
	{
		return;
	}

	AElevator* elevator = UCoreSystem::Get().GetElevator();

	if (elevator != nullptr)
	{
		static const int nameSize = 150;
		static const ImVec2 buttonSize = ImVec2(80, 20);

		ImGui::Text(TCHAR_TO_ANSI(*elevator->GetName()));

		ImGui::SameLine(nameSize);

		if (ImGui::Button("Down", buttonSize))
		{
			elevator->MoveDown(1);
		}

		ImGui::SameLine();

		if (ImGui::Button("Up", buttonSize))
		{
			elevator->MoveUp(1);
		}

		if (ImGui::Button("Destroy"))
		{
			elevator->Destroy();
		}
	}
}

void UDebugWindow::DrawSportDatabase()
{
	USportDataHandler& sportData = UCoreSystem::Get().GetSportDataHandler();

	ImGui::BeginTabBar("TeamsTabBar");
	if (ImGui::BeginTabItem("Cricket"))
	{
		DrawSportData(sportData, Sport::Cricket);
		ImGui::EndTabItem();
	}
	if (ImGui::BeginTabItem("Football"))
	{
		DrawSportData(sportData, Sport::Football);
		ImGui::EndTabItem();
	}
	if (ImGui::BeginTabItem("F1"))
	{
		DrawSportData(sportData, Sport::Formula1);
		ImGui::EndTabItem();
	}
	if (ImGui::BeginTabItem("Team creation"))
	{
		DrawCreateTeamMenu();
		ImGui::EndTabItem();
	}
	if (ImGui::BeginTabItem("Player creation"))
	{
		DrawCreatePlayerMenu(sportData);
		ImGui::EndTabItem();
	}
	ImGui::EndTabBar();
}

void UDebugWindow::DrawSportData(USportDataHandler& sportData, Sport sport)
{
	static int nameSize = 150;
	static int scoreSize = 200;
	static int nationalitySize = 140;
	static int barSize = 10;

	const TArray<UTeam*>& teams = sportData.GetTeams(sport);

	if (sport == Sport::Formula1)
	{
		if (ImGui::Button("Update from API"))
		{
			UCoreSystem::Get().GetF1Api()->ImportDataToSportHandler();
		}
	}

	ImGui::Text("Team name");
	ImGui::SameLine(nameSize + barSize);
	ImGui::Text("|");
	ImGui::SameLine(nameSize + barSize + barSize);

	ImGui::Text("score");
	ImGui::SameLine(nameSize + barSize + barSize + scoreSize + barSize);
	ImGui::Text("|");
	ImGui::SameLine(nameSize + barSize + barSize + scoreSize + barSize + barSize);

	ImGui::Text("nationality");

	ImGui::Text("====================================================================");

	bool showAdditionalTeamInfo = false;
	for (UTeam* team : teams)
	{
		if (m_selectedTeam == team)
		{
			showAdditionalTeamInfo = true;
		}
		if (ImGui::Button(TCHAR_TO_ANSI(*team->GetName()), ImVec2(nameSize, 20)))
		{
			m_selectedTeam = team;
			showAdditionalTeamInfo = true;
		}
		ImGui::SameLine(nameSize + barSize);
		ImGui::Text("|");
		ImGui::SameLine(nameSize + barSize + barSize);

		ImGui::PushID(UUtility::FStringToCharPtr(*FString::Printf(TEXT("%s_score"), *team->GetName())));
		ImGui::PushItemWidth(scoreSize);
		float score = team->GetScore();
		ImGui::SliderFloat("", &score, 0, sportData.GetHighestScoreInSport(Sport::Formula1));
		ImGui::PopItemWidth();
		ImGui::PopID();

		ImGui::SameLine(nameSize + barSize + barSize + scoreSize + barSize);
		ImGui::Text("|");
		ImGui::SameLine(nameSize + barSize + barSize + scoreSize + barSize + barSize);
		ImGui::Text(TCHAR_TO_ANSI(*team->GetNationalityAsString()));
	}

	if (showAdditionalTeamInfo && m_selectedTeam != nullptr)
	{
		ImGui::Separator();

		ImGui::Text(TCHAR_TO_ANSI(*m_selectedTeam->GetName()));

		ImGui::Indent();

		ImGui::Text("Players");
		const TArray<USportPlayer*>& players = m_selectedTeam->GetPlayers();

		DrawPlayersTable(sportData, players, sport);

		ImGui::Unindent();
	}
}

void UDebugWindow::DrawPlayersTable(USportDataHandler& sportData, const TArray<USportPlayer*>& players, Sport sport)
{
	static int nrSize = 40;
	static int nameSize = 200;
	static int scoreSize = 200;
	static int nationalitySize = 140;
	static int barSize = 10;

	ImGui::Text("Nr.");
	ImGui::SameLine(nrSize + barSize);
	ImGui::Text("|");
	ImGui::SameLine(nrSize + barSize * 2);

	ImGui::Text("Name");
	ImGui::SameLine(nrSize + nameSize + barSize * 3);
	ImGui::Text("|");
	ImGui::SameLine(nrSize + nameSize + barSize * 4);

	ImGui::Text("score");
	ImGui::SameLine(nrSize + nameSize + scoreSize + barSize * 5);
	ImGui::Text("|");
	ImGui::SameLine(nrSize + nameSize + scoreSize + barSize * 6);

	ImGui::Text("nationality");

	ImGui::Text("=========================================================================================");

	bool showAdditionalPlayerInfo = false;
	for (USportPlayer* player : players)
	{
		UF1Driver* driver = static_cast<UF1Driver*>(player);

		if (m_selectedPlayer == player)
		{
			showAdditionalPlayerInfo = true;
		}

		ImGui::Text(TCHAR_TO_ANSI(*driver->GetNumberAsString()));
		ImGui::SameLine(nrSize + barSize);
		ImGui::Text("|");
		ImGui::SameLine(nrSize + barSize * 2);

		if (ImGui::Button(TCHAR_TO_ANSI(*player->GetFullName()), ImVec2(nameSize, 20)))
		{
			m_selectedPlayer = player;
			showAdditionalPlayerInfo = true;
		}
		ImGui::SameLine(nrSize + nameSize + barSize * 3);
		ImGui::Text("|");
		ImGui::SameLine(nrSize + nameSize + barSize * 4);

		ImGui::PushID(UUtility::FStringToCharPtr(*FString::Printf(TEXT("%s_score"), *driver->GetFullName())));
		ImGui::PushItemWidth(scoreSize);
		float score = driver->championshipPoints;
		ImGui::SliderFloat("", &score, 0, sportData.GetHighestDriverScoreF1());
		ImGui::PopItemWidth();
		ImGui::PopID();

		ImGui::SameLine(nrSize + nameSize + scoreSize + barSize * 5);
		ImGui::Text("|");
		ImGui::SameLine(nrSize + nameSize + scoreSize + barSize * 6);

		ImGui::Text(TCHAR_TO_ANSI(*driver->GetNationalityAsString()));
	}

	if (showAdditionalPlayerInfo && m_selectedPlayer != nullptr)
	{
		UF1Driver* driver = static_cast<UF1Driver*>(m_selectedPlayer);

		ImGui::Separator();
		ImGui::Indent();
		ImGui::Text(TCHAR_TO_ANSI(*driver->GetFullName()));
		ImGui::NewLine();

		ImGui::Text("Number: ");
		ImGui::SameLine();
		ImGui::Text(TCHAR_TO_ANSI(*driver->GetNumberAsString()));

		ImGui::Text("Score: ");
		ImGui::SameLine();
		ImGui::Text(TCHAR_TO_ANSI(*FString::FromInt(driver->championshipPoints)));

		ImGui::Text("Nationality: ");
		ImGui::SameLine();
		ImGui::Text(TCHAR_TO_ANSI(*driver->GetNationalityAsString()));

		FDateTime dob = driver->GetDateOfBirth();
		ImGui::Text("Date of Birth: ");
		ImGui::SameLine();
		ImGui::Text(TCHAR_TO_ANSI(*FString::Printf(TEXT("%d/%d/%d"), dob.GetDay(), dob.GetMonth(), dob.GetYear())));

		ImGui::Unindent();
	}
}

void UDebugWindow::DrawCreateTeamMenu()
{
	ImGui::Text("Team name");
	ImGui::PushID("TeamName");
	ImGui::InputText("", m_newTeam.teamName, TeamData::nameLength);
	ImGui::PopID();

	ImGui::NewLine();

	int currentItem = static_cast<int>(m_newTeam.sport);

	ImGui::Text("Sport");
	ImGui::PushID("Sport");
	if (ImGui::ListBox("", &currentItem, m_sportsList, 3))
	{
		m_newTeam.sport = static_cast<Sport>(currentItem);
	}
	ImGui::PopID();

	ImGui::NewLine();
	
	ImGui::Text("Nationality");
	ImGui::PushID("Nationality");
	ImGui::InputText("", m_newTeam.nationality, TeamData::nameLength);
	ImGui::PopID();

	ImGui::NewLine();

	ImGui::Text("Score");
	ImGui::PushID("Score");
	ImGui::SliderFloat("", &m_newTeam.score, 0, 500);
	ImGui::PopID();
	
	ImGui::NewLine();

	if (ImGui::Button("Create Team"))
	{
		FString name = UUtility::CharPtrToFString(m_newTeam.teamName);
		UTeam::Make(name, m_newTeam.sport, m_newTeam.score, UUtility::CharPtrToFString(m_newTeam.nationality));
	}
}

void UDebugWindow::DrawCreatePlayerMenu(USportDataHandler& sportData)
{
	int currentItem = static_cast<int>(m_newPlayer.sport);

	ImGui::Text("Sport");
	ImGui::PushID("Sport");
	if (ImGui::ListBox("", &currentItem, m_sportsList, 3))
	{
		m_newPlayer.sport = static_cast<Sport>(currentItem);
		m_newPlayer.team = nullptr;
	}
	ImGui::PopID();

	ImGui::NewLine();

	ImGui::PushItemWidth(200);

	ImGui::Text("Name");
	ImGui::PushID("first_name");
	ImGui::InputText("", m_newPlayer.firstName, PlayerData::nameLength);
	ImGui::PopID();

	ImGui::SameLine();

	ImGui::PushID("last_name");
	ImGui::InputText("Last name", m_newPlayer.lastName, PlayerData::nameLength);
	ImGui::PopID();

	ImGui::Text("Display name");
	ImGui::PushID("display_name");
	ImGui::InputText("", m_newPlayer.displayName, PlayerData::nameLength);
	ImGui::PopID();

	ImGui::PopItemWidth();

	ImGui::NewLine();

	ImGui::Text("Number");
	ImGui::PushID("Number");
	ImGui::SliderInt("", &m_newPlayer.number, 0, 99);
	ImGui::PopID();

	ImGui::NewLine();

	ImGui::PushItemWidth(100);

	ImGui::Text("Date of Birth");
	ImGui::PushID("dob");
	ImGui::SliderInt("D", &m_newPlayer.dob_day, 1, 31);
	ImGui::SliderInt("M", &m_newPlayer.dob_month, 1, 12);
	ImGui::SliderInt("Y", &m_newPlayer.dob_year, 1950, 2021);
	ImGui::PopID();

	ImGui::PopItemWidth();

	if (ImGui::TreeNode("Teams"))
	{
		const TArray<UTeam*> teams = sportData.GetTeams(m_newPlayer.sport);
		static const ImVec2 btnSize = ImVec2(120, 30);
		static const int columns = 5;

		UTeam* currentlySelected = m_newPlayer.team;

		int index = 0;
		for (UTeam* team : teams)
		{
			if (team == currentlySelected)
			{
				ImGui::PushStyleColor(0, ImVec4(0, 1, 0, 1));
			}

			char* name = UUtility::FStringToCharPtr(team->GetName());
			if (ImGui::Button(name, btnSize))
			{
				m_newPlayer.team = team;
			}

			if (team == currentlySelected)
			{
				ImGui::PopStyleColor(1);
			}

			bool isNewColumn = (index + 1) % columns == 0;
			bool isLast = index + 1 == teams.Num();
			if (!isNewColumn && !isLast)
			{
				ImGui::SameLine();
			}
			++index;
		}
		ImGui::TreePop();
	}

	ImGui::NewLine();

	ImGui::Text("Nationality");
	ImGui::PushID("Nationality");
	ImGui::InputText("", m_newPlayer.nationality, PlayerData::nameLength);
	ImGui::PopID();

	ImGui::NewLine();

	if (m_newPlayer.sport == Sport::Formula1)
	{
		ImGui::Text("Score");
		ImGui::PushID("Score");
		ImGui::SliderFloat("", &m_newPlayer.driverScoreF1, 0, 500);
		ImGui::PopID();

		ImGui::NewLine();
	}

	if (m_newPlayer.team == nullptr)
	{
		ImGui::Text("Team is not assigned - cannot create plaeyr");
	}
	else
	{
		if (ImGui::Button("Create player"))
		{
			USportPlayer* player = nullptr;

			if (m_newPlayer.sport == Sport::Formula1)
			{
				UF1Driver& driver = UF1Driver::Make(m_newPlayer.firstName, m_newPlayer.lastName, m_newPlayer.displayName, *m_newPlayer.team);
				driver.championshipPoints = m_newPlayer.driverScoreF1;
				player = static_cast<USportPlayer*>(&driver);
			}
			else
			{
				player = &USportPlayer::Make(m_newPlayer.firstName, m_newPlayer.lastName, m_newPlayer.displayName, *m_newPlayer.team);
			}

			if (player != nullptr)
			{
				player->nationalityAsString = UUtility::CharPtrToFString(m_newPlayer.nationality);
				player->SetNumber(m_newPlayer.number);
				player->SetDateOfBirth(FDateTime(m_newPlayer.dob_year, m_newPlayer.dob_month, m_newPlayer.dob_day));
			}
		}
	}
}

void UDebugWindow::ImGuiSliderVector(const char* label, FVector& vector, float xLimit, float yLimit, float zLimit)
{
	ImGui::PushID(label);
	ImGui::Text(label);

	FVector copy = FVector(vector.X, vector.Y, vector.Z);

	ImGui::PushItemWidth(200);

	ImGui::PushID("X");
	ImGui::SliderFloat("", &vector.X, -xLimit, xLimit);
	ImGui::PopID();

	ImGui::SameLine();

	ImGui::PushID("Y");
	ImGui::SliderFloat("", &vector.Y, -yLimit, yLimit);
	ImGui::PopID();

	ImGui::SameLine();

	ImGui::PushID("Z");
	ImGui::SliderFloat("", &vector.Z, -zLimit, zLimit);
	ImGui::PopID();

	ImGui::PopItemWidth();
	ImGui::PopID();
}
#endif
