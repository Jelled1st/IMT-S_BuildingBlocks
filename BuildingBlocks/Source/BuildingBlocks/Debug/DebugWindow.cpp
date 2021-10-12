// Fill out your copyright notice in the Description page of Project Settings.


#include "DebugWindow.h"

#include "..\..\..\Plugins\ImGui\Source\ImGui\Private\ImGuiPrivatePCH.h"
#include "..\..\..\Plugins\ImGui\Source\ImGui\Public\ImGuiModuleProperties.h"

#include "Debug.h"

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
#endif
