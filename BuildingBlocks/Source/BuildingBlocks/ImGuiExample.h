// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ImGuiCommon.h"

#if WITH_IMGUI
#include "..\..\Plugins\ImGui\Source\ImGui\Public\ImGuiDelegates.h"
class FImGuiModuleProperties;

class BUILDINGBLOCKS_API ImGuiExample
{
public:
	ImGuiExample();
	~ImGuiExample();

	void DrawControls(int32 ContextIndex);

private:
	ImVec4 clearColor = ImColor{ 114, 144, 154 };

	int32 showDemoWindowMask = 0;
	int32 showAnotherWindowMask = 0;

	int32 demoWindowCounter = 0;
	uint32 lastDemoWindowFrameNumber = 0;
};

#endif