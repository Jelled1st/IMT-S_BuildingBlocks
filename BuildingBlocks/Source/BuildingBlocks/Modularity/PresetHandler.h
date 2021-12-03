// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "PresetHandler.generated.h"

UCLASS()
class BUILDINGBLOCKS_API UPresetHandler : public UObject
{
	GENERATED_BODY()
	
public:
	UPresetHandler();
	~UPresetHandler();

	bool LoadPresetsFromFile();
	bool SavePresetsToFile();

private:
	const static FString m_subFolders;
	const static FString m_filePath;

	FString GetFullDirectory();

	TMap<FString, FString> m_presetsAsJson;
};
