// Fill out your copyright notice in the Description page of Project Settings.


#include "DebugWindow.h"

#include "..\..\..\Plugins\ImGui\Source\ImGui\Private\ImGuiPrivatePCH.h"
#include "..\..\..\Plugins\ImGui\Source\ImGui\Public\ImGuiModuleProperties.h"

#include "Debug.h"
#include "..\ModularObject.h"
#include "..\ModularitySystem.h"
#include "..\CoreSystem.h"
#include "Dom/JsonObject.h"
#include "Json.h"
#include "Containers/Map.h"

#include <string>

DebugWindow::DebugWindow()
{
}

DebugWindow::~DebugWindow()
{
}

void DebugWindow::Start()
{
#if WITH_IMGUI
	FImGuiDelegates::OnWorldDebug().AddLambda([this]() { DrawWindow(); });
#endif
}

#if WITH_IMGUI
void DebugWindow::DrawWindow()
{
	ImGui::Begin("Debug Window");

	if (ImGui::BeginTabBar("TabBar"))
	{
		if (ImGui::BeginTabItem("Operator Controls"))
		{
			DrawOperatorControls();
			ImGui::EndTabItem();
		}

		if (ImGui::BeginTabItem("Example"))
		{
			ImGui::Text("Hello world!");

			if (ImGui::Button("Debug::Log('Hello World')"))
			{
				Debug::Log("Hello World");
			}

			ImGui::SliderFloat("Slider", &m_slideValue, -5, 5);

			ImGui::EndTabItem();
		}

		ImGui::EndTabBar();
	}

	ImGui::End();
}

void DebugWindow::DrawOperatorControls()
{
	if (ImGui::TreeNode("Presets"))
	{
		DrawPresetMenu();
		ImGui::TreePop();
	}

	if (ImGui::BeginTabBar("OperatorControlsBar"))
	{
		TArray<TSharedPtr<AModularObject>> modularObjs = UCoreSystem::Get().GetModularitySystem()->GetRegisteredObjects();
		
		for (TSharedPtr<AModularObject> obj : modularObjs)
		{
			FString name = obj.Get()->GetName();
			if (ImGui::BeginTabItem(TCHAR_TO_ANSI(*name)))
			{
				m_selectedObject = obj;

				ImGui::EndTabItem();
			}
		}

		if (m_selectedObject.IsValid())
		{
			DrawObjectControls(*m_selectedObject.Get());
		}

		ImGui::EndTabBar();
	}
}

void DebugWindow::DrawPresetMenu()
{
	if (ImGui::Button("Save preset"))
	{
		TSharedPtr<FJsonObject> jsonWriteObject = MakeShareable(new FJsonObject);

		TArray<TSharedPtr<AModularObject>> modularObjs = UCoreSystem::Get().GetModularitySystem()->GetRegisteredObjects();
		for (TSharedPtr<AModularObject> obj : modularObjs)
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

			TArray<TSharedPtr<AModularObject>> modularObjs = UCoreSystem::Get().GetModularitySystem()->GetRegisteredObjects();
			TMap<FString, TSharedPtr<AModularObject>> objectByName;

			for (TSharedPtr<AModularObject> obj : modularObjs)
			{
				objectByName.Add(obj->GetName(), obj);
			}

			TMap<FString, TSharedPtr<FJsonValue>> globalJsonValues = jsonLoadObject->Values;

			for ( TPair<FString, TSharedPtr<FJsonValue>> objectJsonValue : globalJsonValues)
			{
				if (objectJsonValue.Value->Type != EJson::Object)
				{
					Debug::Log(FString::Printf(TEXT("Unexpected type in json object {0}"), *objectJsonValue.Key));
				}
				else
				{
					TSharedPtr<AModularObject> modularObject = objectByName[objectJsonValue.Key];
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

void DebugWindow::DrawObjectControls(AModularObject& object)
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

void DebugWindow::DrawObjectTransform(AModularObject& object)
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

void DebugWindow::ImGuiSliderVector(const char* label, FVector& vector, float xLimit, float yLimit, float zLimit)
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
