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

#include <string>

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
	ImGui::Begin("Debug Window");

	if (ImGui::BeginTabBar("TabBar"))
	{
		if (ImGui::BeginTabItem("Operator Controls"))
		{
			DrawOperatorControls();
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
	if (ImGui::TreeNode("Presets"))
	{
		DrawPresetMenu();
		ImGui::TreePop();
	}

	if (ImGui::BeginTabBar("OperatorControlsBar"))
	{
		TArray<AModularObject*>& modularObjs = UCoreSystem::Get().GetModularitySystem()->GetRegisteredObjects();
		
		for (AModularObject* obj : modularObjs)
		{
			FString name = obj->GetName();
			if (ImGui::BeginTabItem(TCHAR_TO_ANSI(*name)))
			{
				m_selectedObject = obj;

				ImGui::EndTabItem();
			}
		}

		if (m_selectedObject == nullptr)
		{
			DrawObjectControls(*m_selectedObject);
		}

		ImGui::EndTabBar();
	}
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

			for ( TPair<FString, TSharedPtr<FJsonValue>> objectJsonValue : globalJsonValues)
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

		switch(parameterType)
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

			ImGui::InputText(TCHAR_TO_ANSI(*parameter.Key), stringAsChar , 1000);

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
	static int nameSize = 140;
	static int scoreSize = 50;
	static int nationalitySize = 140;
	static int barSize = 10;

	TArray<TSharedPtr<UTeam>> teams = sportData.GetTeams(sport);

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
	for (TSharedPtr<UTeam> team : teams)
	{
		if (team->score > highestScore)
		{
			highestScore = team->score;
		}
	}

	for (TSharedPtr<UTeam> team : teams)
	{
		ImGui::Text(UUtility::FStringToCharPtr(team->teamName));
		ImGui::SameLine(nameSize + barSize);
		ImGui::Text("|");
		ImGui::SameLine(barSize);

		ImGui::PushID(UUtility::FStringToCharPtr(team->teamName));
		ImGui::PushItemWidth(scoreSize);
		float score = team->score;
		ImGui::SliderFloat("", &score, 0, highestScore);
		ImGui::PopItemWidth();
		ImGui::PopID();

		ImGui::SameLine(scoreSize + barSize);
		ImGui::Text("|");
		ImGui::SameLine(barSize);

		ImGui::Text(UUtility::FStringToCharPtr("Unknown"));
		ImGui::SameLine(nationalitySize);
		ImGui::Text("|");
	}
}

void UDebugWindow::DrawCreateTeamMenu()
{

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
