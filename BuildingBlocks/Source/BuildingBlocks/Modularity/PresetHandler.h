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

	bool SavePreset(const FString& presetName);
	bool LoadPreset(const FString& presetName);

	bool LoadPresetsFromFile();
	bool SavePresetsToFile();

	// Can be optimized by not adding to the array
	TArray<FString> GetPresetNames() const;

private:
	const static FString m_subFolders;
	const static FString m_filePath;

	FString GetFullDirectory();

	bool SaveStringToFile(const FString& presetsAsJson);
	bool LoadStringFromFile(FString& presetsAsJson);

	TMap<FString, FString> m_presetsAsJson;
};
