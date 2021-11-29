// Fill out your copyright notice in the Description page of Project Settings.


#include "DebugWindow.h"

#include "../../../Plugins/ImGui/Source/ImGui/Private/ImGuiPrivatePCH.h"
#include "../../../Plugins/ImGui/Source/ImGui/Public/ImGuiModuleProperties.h"

#include "Debug.h"
#include "../Modularity/ModularObject.h"
#include "../Modularity/ModularitySystem.h"
#include "../Core/CoreSystem.h"
#include "Dom/JsonObject.h"
#include "Json.h"
#include "Containers/Map.h"

#include "../Gameplay/Elevator.h"

#include <string>

bool UDebugWindow::debugWindowEnabled = true;

UDebugWindow::UDebugWindow()
{
}

UDebugWindow::~UDebugWindow()
{
}

void UDebugWindow::Start()
{
#if WITH_IMGUI
	FImGuiDelegates::OnWorldDebug().AddLambda([this]() { DrawWindow(); });
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

	ImGui::EndGroup();
}

void UDebugWindow::DrawPresetMenu()
{
	if (ImGui::Button("Save preset"))
	{
		TSharedPtr<FJsonObject> jsonWriteObject = MakeShareable(new FJsonObject);

		TArray<AModularObject*>& modularObjs = UCoreSystem::Get().GetModularitySystem()->GetRegisteredObjects();
		for (AModularObject* obj : modularObjs)
		{
			FString name = obj->GetName();

			TMap<FString, TPair<AModularObject::ParameterType, void*>>& parameters = obj->GetParameters();

			TSharedPtr<FJsonObject> jsonSubObject = MakeShareable(new FJsonObject);
			jsonSubObject->SetStringField("Name", name);

			for (TPair<FString, TPair<AModularObject::ParameterType, void* >> parameter : parameters)
			{
				FString parameterName = parameter.Key;
				AModularObject::ParameterType parameterType = parameter.Value.Key;
				switch (parameterType)
				{
				case AModularObject::ParameterType::Bool:
				{
					bool boolValue = *reinterpret_cast<bool*>(parameter.Value.Value);
					jsonSubObject->SetBoolField(parameterName, boolValue);
					break;
				}
				case AModularObject::ParameterType::String:
				{
					FString stringValue = *reinterpret_cast<FString*>(parameter.Value.Value);
					jsonSubObject->SetStringField(parameterName, stringValue);
					break;
				}
				case AModularObject::ParameterType::Double:
				{
					double doubleValue = *reinterpret_cast<double*>(parameter.Value.Value);
					jsonSubObject->SetNumberField(parameterName, static_cast<double>(doubleValue));
					break;
				}

				}
			}

			jsonWriteObject->SetObjectField(name, jsonSubObject);
		}
		TSharedRef<TJsonWriter<>> JsonWriter = TJsonWriterFactory<>::Create(&m_jsonString);
		FJsonSerializer::Serialize(jsonWriteObject.ToSharedRef(), JsonWriter);
	}

	ImGui::SameLine();

	if (ImGui::Button("Load preset"))
	{
		TSharedPtr<FJsonObject> jsonLoadObject = MakeShareable(new FJsonObject);
		TSharedRef<TJsonReader<>> jsonReader = TJsonReaderFactory<>::Create(m_jsonString);

		if (!FJsonSerializer::Deserialize(jsonReader, jsonLoadObject))
		{
			m_deserializeSucceeded = false;
		}
		else
		{
			m_deserializeSucceeded = true;

			TArray<AModularObject*> modularObjs = UCoreSystem::Get().GetModularitySystem()->GetRegisteredObjects();
			TMap<FString, AModularObject*> objectByName;

			for (AModularObject* obj : modularObjs)
			{
				objectByName.Add(obj->GetName(), obj);
			}

			TMap<FString, TSharedPtr<FJsonValue>> globalJsonValues = jsonLoadObject->Values;

			for (TPair<FString, TSharedPtr<FJsonValue>> objectJsonValue : globalJsonValues)
			{
				if (objectJsonValue.Value->Type != EJson::Object)
				{
					UDebug::Log(FString::Printf(TEXT("Unexpected type in json object {0}"), *objectJsonValue.Key));
				}
				else
				{
					AModularObject* modularObject = objectByName[objectJsonValue.Key];
					TSharedPtr<FJsonObject> jsonObject = objectJsonValue.Value->AsObject();

					TMap<FString, TSharedPtr<FJsonValue>> jsonValues = jsonObject->Values;

					for (TPair<FString, TSharedPtr<FJsonValue>> jsonValue : jsonValues)
					{
						FString valueName = jsonValue.Key;
						TSharedPtr<FJsonValue> abstractValue = jsonValue.Value;

						EJson type = abstractValue->Type;

						if (type == EJson::String)
						{
							FString value = abstractValue->AsString();
							modularObject->SetParameterValue(valueName, value);
						}

						if (type == EJson::Boolean)
						{
							bool value = abstractValue->AsBool();
							modularObject->SetParameterValue(valueName, value);
						}

						if (type == EJson::Number)
						{
							double value = abstractValue->AsNumber();
							modularObject->SetParameterValue(valueName, value);
						}
					}
				}
			}
		}
	}

	if (!m_deserializeSucceeded)
	{
		ImGui::Text("Deserialization failed!");
		ImGui::NewLine();
	}

	if (ImGui::TreeNode("Current json"))
	{
		ImGui::Text(TCHAR_TO_ANSI(*m_jsonString));

		ImGui::TreePop();
	}
}

void UDebugWindow::DrawObjectControls(AModularObject& object)
{
	ImGui::Text(TCHAR_TO_ANSI(*object.GetName()));

	static float arrowButtonSpacing = 40;
	static float textSpacing = 160;

	DrawObjectTransform(object);

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

	TMap<FString, TPair<AModularObject::ParameterType, void*>>& parameters = object.GetParameters();

	for (TPair<FString, TPair<AModularObject::ParameterType, void* >> parameter : parameters)
	{
		AModularObject::ParameterType parameterType = parameter.Value.Key;

		switch (parameterType)
		{
		case AModularObject::ParameterType::Bool:
		{
			ImGui::Checkbox(TCHAR_TO_ANSI(*parameter.Key), reinterpret_cast<bool*>(parameter.Value.Value));
			break;
		}
		case AModularObject::ParameterType::String:
		{
			FString* stringPtr = reinterpret_cast<FString*>(parameter.Value.Value);
			char* stringAsChar = TCHAR_TO_ANSI(*(*stringPtr));

			ImGui::InputText(TCHAR_TO_ANSI(*parameter.Key), stringAsChar, 1000);

			FString outValue = FString(stringAsChar);

			*stringPtr = outValue;
			break;
		}
		case AModularObject::ParameterType::Double:
		{
			double* doubleValue = reinterpret_cast<double*>(parameter.Value.Value);
			float floatValue = static_cast<float>(*doubleValue);

			ImGui::SliderFloat(TCHAR_TO_ANSI(*parameter.Key), &floatValue, -100000, 100000);

			*doubleValue = static_cast<double>(floatValue);

			break;
		}

		}
	}
}

void UDebugWindow::DrawObjectTransform(AModularObject& object)
{
	FTransform transform = object.GetActorTransform();

	FVector position = transform.GetLocation();
	FVector rotation = transform.GetRotation().Euler();
	FVector scale = transform.GetScale3D();

	if (ImGui::TreeNode("Transform"))
	{
		ImGuiSliderVector("Position", position);
		ImGuiSliderVector("Rotation", rotation, 180, 180, 180);
		ImGuiSliderVector("Scale", scale, 10, 10, 10);
		ImGui::TreePop();
	}

	transform.SetLocation(position);
	transform.SetRotation(FQuat::MakeFromEuler(rotation));
	transform.SetScale3D(scale);

	object.SetActorTransform(transform);
}

void UDebugWindow::DrawElevatorControls()
{
	if (!UCoreSystem::Exists())
	{
		return;
	}

	AElevator* elevator = UCoreSystem::Get().GetElevator();

	if (elevator == nullptr)
	{
		return;
	}

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

	float highestScore = 0;
	for (UTeam* team : teams)
	{
		if (team->GetScore() > highestScore)
		{
			highestScore = team->GetScore();
		}
	}

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
		ImGui::SliderFloat("", &score, 0, highestScore);
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

		DrawPlayersTable(players, sport);

		ImGui::NewLine();

		if (ImGui::TreeNode("Create player"))
		{
			ImGui::Indent();

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

			if (ImGui::Button("Create player"))
			{
				USportPlayer::Make(m_newPlayer.firstName, m_newPlayer.lastName, m_newPlayer.displayName, *m_selectedTeam);
			}

			ImGui::Unindent();
			ImGui::TreePop();
		}

		ImGui::Unindent();
	}
}

void UDebugWindow::DrawPlayersTable(const TArray<USportPlayer*>& players, Sport sport)
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
		if (m_selectedPlayer == player)
		{
			showAdditionalPlayerInfo = true;
		}

		ImGui::Text(TCHAR_TO_ANSI(*player->GetNumberAsString()));
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

		ImGui::PushID(UUtility::FStringToCharPtr(*FString::Printf(TEXT("%s_score"), *player->GetName())));
		ImGui::PushItemWidth(scoreSize);
		static float score;
		ImGui::SliderFloat("", &score, 0, 0);
		ImGui::PopItemWidth();
		ImGui::PopID();

		ImGui::SameLine(nrSize + nameSize + scoreSize + barSize * 5);
		ImGui::Text("|");
		ImGui::SameLine(nrSize + nameSize + scoreSize + barSize * 6);

		ImGui::Text(TCHAR_TO_ANSI(*player->nationalityAsString));
	}
}

void UDebugWindow::DrawCreateTeamMenu()
{
	bool isCricket = m_newTeam.sport == Sport::Cricket;
	bool isFootball = m_newTeam.sport == Sport::Football;
	bool isF1 = m_newTeam.sport == Sport::Formula1;
	if (ImGui::Checkbox("Cricket", &isCricket))
	{
		m_newTeam.sport = Sport::Cricket;
	}
	if (ImGui::Checkbox("Football", &isFootball))
	{
		m_newTeam.sport = Sport::Football;
	}
	if (ImGui::Checkbox("Formula1", &isF1))
	{
		m_newTeam.sport = Sport::Formula1;
	}

	ImGui::InputText("Team Name", m_newTeam.teamName, m_newTeam.nameLength);

	TArray<Country> countries = UUtility::CreateCountryArray();

	static const int rowElements = 5;
	if (ImGui::TreeNode("Select nationality"))
	{
		ImGui::Separator();
		for (int i = 0; i < countries.Num(); ++i)
		{
			Country country = countries[i];
			bool currentlySelected = m_newTeam.selectedNationality == country;

			FString countryNameFString = UUtility::EnumToString(TEXT("Country"), static_cast<int>(country));
			if (currentlySelected)
			{
				ImGui::PushStyleColor(0, ImVec4(0, 1, 0, 1));
				countryNameFString = FString::Printf(TEXT("X %s X"), *countryNameFString);
			}

			char* countryName = UUtility::FStringToCharPtr(*countryNameFString);
			if (ImGui::Button(countryName, ImVec2(140, 30)))
			{
				m_newTeam.selectedNationality = country;
			}

			if (currentlySelected)
			{
				ImGui::PopStyleColor();
			}

			bool isLast = i == countries.Num() - 1;
			if ((i + 1) % rowElements != 0 && !isLast)
			{
				ImGui::SameLine();
			}
		}

		ImGui::Separator();
		ImGui::TreePop();
	}

	ImGui::SliderFloat("Score", &m_newTeam.score, 0, 500);

	if (ImGui::Button("Create Team"))
	{
		FString name = UUtility::CharPtrToFString(m_newTeam.teamName);
		UTeam::Make(name, m_newTeam.sport, m_newTeam.score, UUtility::EnumToString(TEXT("Country"), static_cast<int>(m_newTeam.selectedNationality)), m_newTeam.selectedNationality);
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
