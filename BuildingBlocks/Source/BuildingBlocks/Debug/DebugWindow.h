// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "..\ImGuiCommon.h"

#if WITH_IMGUI
#include "..\..\..\Plugins\ImGui\Source\ImGui\Public\ImGuiDelegates.h"
class FImGuiModuleProperties;
#endif

class BUILDINGBLOCKS_API DebugWindow
{
public:
	DebugWindow();
	~DebugWindow();

	void Start();

#if WITH_IMGUI
	void DrawWindow();
#endif

private:
	float m_slideValue = 0;
};
