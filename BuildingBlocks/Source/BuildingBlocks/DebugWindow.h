// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ImGuiCommon.h"
#include "DebugWindow.generated.h"

#if WITH_IMGUI
#include "..\..\Plugins\ImGui\Source\ImGui\Public\ImGuiDelegates.h"
class FImGuiModuleProperties;
#endif

UCLASS()
class BUILDINGBLOCKS_API ADebugWindow : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ADebugWindow();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

#if WITH_IMGUI
	void DrawWindow();
#endif
};
