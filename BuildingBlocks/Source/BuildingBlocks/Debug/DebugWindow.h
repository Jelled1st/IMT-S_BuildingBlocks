// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ImGuiCommon.h"

#if WITH_IMGUI
#include "..\..\..\Plugins\ImGui\Source\ImGui\Public\ImGuiDelegates.h"
class FImGuiModuleProperties;
class AModularObject;
#endif

#include "DebugWindow.generated.h"

UCLASS()
class BUILDINGBLOCKS_API UDebugWindow : public UObject
{
	GENERATED_BODY()

public:
	UDebugWindow();
	~UDebugWindow();

	void Start();

#if WITH_IMGUI
	void DrawWindow();

	static void ImGuiSliderVector(const char* label, FVector& vector, float xLimit = 1000, float yLimit = 1000, float zLimit = 1000);
#endif

private:
	float m_slideValue = 0;
	bool m_isSaveWindowOpen = false;

#if WITH_IMGUI
	void DrawOperatorControls();
	void DrawObjectControls(AModularObject& object);
	void DrawObjectTransform(AModularObject& object);

	void DrawPresetMenu();
	FString m_jsonString = "";
	bool m_deserializeSucceeded = true;
	TSharedPtr<AModularObject> m_selectedObject;
#endif
};
