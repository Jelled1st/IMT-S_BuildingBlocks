// Fill out your copyright notice in the Description page of Project Settings.


#include "DebugWindow.h"

#include "..\..\..\Plugins\ImGui\Source\ImGui\Private\ImGuiPrivatePCH.h"
#include "..\..\..\Plugins\ImGui\Source\ImGui\Public\ImGuiModuleProperties.h"

#include "Debug.h"
#include "..\ModularObject.h"
#include "..\ModularitySystem.h"
#include "..\CoreSystem.h"

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
}
#endif
