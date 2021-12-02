// Fill out your copyright notice in the Description page of Project Settings.


#include "PresetHandler.h"
#include "../Debug/Debug.h"

const FString UPresetHandler::m_filePath = "presets.json";

UPresetHandler::UPresetHandler()
{

}

UPresetHandler::~UPresetHandler()
{

}

bool UPresetHandler::LoadPresetsFromFile()
{
	return false;
}

bool UPresetHandler::SavePresetsToFile()
{
	FString persistentDir = FPlatformProcess::UserSettingsDir();

	IFileManager& fileManager = IFileManager::Get();

	if (!fileManager.DirectoryExists(*persistentDir))
	{
		UDebug::Warning(*FString::Printf(TEXT("Directory '%s' does not exist"), *persistentDir));

		fileManager.MakeDirectory(*persistentDir);
	}

	return false;
}