// Fill out your copyright notice in the Description page of Project Settings.


#include "DebugWindow.h"

#include "..\..\..\Plugins\ImGui\Source\ImGui\Private\ImGuiPrivatePCH.h"
#include "..\..\..\Plugins\ImGui\Source\ImGui\Public\ImGuiModuleProperties.h"

#include "Debug.h"
#include "..\ModularObject.h"
#include "..\ModularitySystem.h"
#include "..\CoreSystem.h"

#include "Math/NumericLimits.h"

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

void DebugWindow::DrawObjectControls(AModularObject& object)
{
	ImGui::Text(TCHAR_TO_ANSI(*object.GetName()));

	static float arrowButtonSpacing = 40;
	static float textSpacing = 100;

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
		case AModularObject::ParameterType::Int:
		{
			ImGui::SliderInt(TCHAR_TO_ANSI(*parameter.Key), reinterpret_cast<int*>(parameter.Value.Value), -100000, 100000);
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
		case AModularObject::ParameterType::Float:
		{
			ImGui::SliderFloat(TCHAR_TO_ANSI(*parameter.Key), reinterpret_cast<float*>(parameter.Value.Value), -100000, 100000);
			break;
		}
		}
	}
}
#endif
