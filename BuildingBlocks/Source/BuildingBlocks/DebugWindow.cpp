// Fill out your copyright notice in the Description page of Project Settings.


#include "..\..\Plugins\ImGui\Source\ImGui\Private\ImGuiPrivatePCH.h"

#include "DebugWindow.h"

#include "..\..\Plugins\ImGui\Source\ImGui\Public\ImGuiModuleProperties.h"


// Sets default values
ADebugWindow::ADebugWindow()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ADebugWindow::BeginPlay()
{
	Super::BeginPlay();

#if WITH_IMGUI
	FImGuiDelegates::OnWorldDebug().AddLambda([this]() { DrawWindow(); });
#endif
}

// Called every frame
void ADebugWindow::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

#if WITH_IMGUI
void ADebugWindow::DrawWindow()
{
	ImGui::Begin("Debug Window");
	ImGui::Text("Hello world!");
	ImGui::End();
}
#endif
