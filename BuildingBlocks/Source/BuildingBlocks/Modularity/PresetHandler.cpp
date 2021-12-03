// Fill out your copyright notice in the Description page of Project Settings.


#include "PresetHandler.h"
#include "../Debug/Debug.h"

const FString UPresetHandler::m_subFolders = "BuildingBlocks";
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

FString UPresetHandler::GetFullDirectory()
{
	FString persistentDir = FPlatformProcess::UserSettingsDir();
	FString fullDir = persistentDir + m_subFolders + "/";
	return fullDir;
}

bool UPresetHandler::SavePresetsToFile()
{
	FString fullDir = GetFullDirectory();
	UDebug::Log(*FString::Printf(TEXT("Writing to directory: %s"), *fullDir));

	IFileManager& fileManager = IFileManager::Get();

	if (!fileManager.DirectoryExists(*fullDir))
	{
		UDebug::Warning(*FString::Printf(TEXT("Directory '%s' does not exist"), *fullDir));

		fileManager.MakeDirectory(*fullDir);
	}

	FString filePath = fullDir + m_filePath;

	if (!fileManager.FileExists(*filePath))
	{
		UDebug::Warning(*FString::Printf(TEXT("File '%s' does not exist"), *filePath));
	}

	return FFileHelper::SaveStringToFile(TEXT("This is overwriting test"), *filePath);
}