// Fill out your copyright notice in the Description page of Project Settings.


#include "PresetHandler.h"
#include "../Debug/Debug.h"
#include "ModularObject.h"
#include "../Core/CoreSystem.h"

const FString UPresetHandler::m_subFolders = "BuildingBlocks";
const FString UPresetHandler::m_filePath = "presets.json";

UPresetHandler::UPresetHandler()
{

}

UPresetHandler::~UPresetHandler()
{

}

bool UPresetHandler::SavePreset(const FString& presetName)
{
	TSharedPtr<FJsonObject> jsonWriteObject = MakeShareable(new FJsonObject);

	const TArray<TWeakObjectPtr<AModularObject>>& modularObjs = UCoreSystem::Get().GetModularitySystem()->GetRegisteredObjects();
	for (TWeakObjectPtr<AModularObject> obj : modularObjs)
	{
		if (!obj.IsValid())
		{
			continue;
		}

		FString name = obj->GetName();

		TMap<FString, TPair<ExposableParameterType, void*>>& parameters = obj->GetParameters();

		TSharedPtr<FJsonObject> jsonSubObject = MakeShareable(new FJsonObject);
		jsonSubObject->SetStringField("Name", name);

		jsonSubObject->SetStringField("mesh", obj->GetMesh().GetName());
		jsonSubObject->SetStringField("material", obj->GetMaterial().GetName());

		for (TPair<FString, TPair<ExposableParameterType, void* >> parameter : parameters)
		{
			FString parameterName = parameter.Key;
			ExposableParameterType parameterType = parameter.Value.Key;
			switch (parameterType)
			{
			case ExposableParameterType::Bool:
			{
				bool boolValue = *reinterpret_cast<bool*>(parameter.Value.Value);
				jsonSubObject->SetBoolField(parameterName, boolValue);
				break;
			}
			case ExposableParameterType::String:
			{
				FString stringValue = *reinterpret_cast<FString*>(parameter.Value.Value);
				jsonSubObject->SetStringField(parameterName, stringValue);
				break;
			}
			case ExposableParameterType::Double:
			{
				double doubleValue = *reinterpret_cast<double*>(parameter.Value.Value);
				jsonSubObject->SetNumberField(parameterName, static_cast<double>(doubleValue));
				break;
			}

			}
		}

		jsonWriteObject->SetObjectField(name, jsonSubObject);
	}

	FString presetAsJson;
	TSharedRef<TJsonWriter<>> JsonWriter = TJsonWriterFactory<>::Create(&presetAsJson);

	if (FJsonSerializer::Serialize(jsonWriteObject.ToSharedRef(), JsonWriter))
	{
		if (m_presetsAsJson.Contains(presetName))
		{
			m_presetsAsJson[presetName] = presetAsJson;
		}
		else
		{
			m_presetsAsJson.Add(presetName, presetAsJson);
		}

		return true;
	}
	else
	{
		return false;
	}
}

bool UPresetHandler::LoadPreset(const FString& presetName)
{
	FString presetAsJson;

	if (m_presetsAsJson.Contains(presetName))
	{
		presetAsJson = m_presetsAsJson[presetName];
	}
	else
	{
		return false;
	}

	TSharedPtr<FJsonObject> jsonLoadObject = MakeShareable(new FJsonObject);
	TSharedRef<TJsonReader<>> jsonReader = TJsonReaderFactory<>::Create(presetAsJson);

	bool isDeserializeSuccessful = false;

	if (FJsonSerializer::Deserialize(jsonReader, jsonLoadObject))
	{
		isDeserializeSuccessful = true;

		const TArray<TWeakObjectPtr<AModularObject>> modularObjs = UCoreSystem::Get().GetModularitySystem()->GetRegisteredObjects();
		TMap<FString, AModularObject*> objectByName;

		for (TWeakObjectPtr<AModularObject> obj : modularObjs)
		{
			if (obj.IsValid())
			{
				objectByName.Add(obj->GetName(), obj.Get());
			}
		}

		TMap<FString, TSharedPtr<FJsonValue>> globalJsonValues = jsonLoadObject->Values;

		for (TPair<FString, TSharedPtr<FJsonValue>> objectJsonValue : globalJsonValues)
		{
			if (objectJsonValue.Value->Type != EJson::Object)
			{
				UDebug::Log(FString::Printf(TEXT("Unexpected type in json object {0}"), *objectJsonValue.Key));
			}
			else
			{
				AModularObject* modularObject = objectByName[objectJsonValue.Key];
				TSharedPtr<FJsonObject> jsonObject = objectJsonValue.Value->AsObject();

				TMap<FString, TSharedPtr<FJsonValue>> jsonValues = jsonObject->Values;

				for (TPair<FString, TSharedPtr<FJsonValue>> jsonValue : jsonValues)
				{
					FString valueName = jsonValue.Key;
					TSharedPtr<FJsonValue> abstractValue = jsonValue.Value;

					EJson type = abstractValue->Type;

					if (type == EJson::String)
					{

						FString value = abstractValue->AsString();

						if (valueName == "mesh")
						{
							modularObject->TrySetMeshByName(value);
						}
						else if (valueName == "material")
						{
							modularObject->TrySetMaterialByName(value);
						}
						else
						{
							modularObject->SetParameterValue(valueName, value);
						}
					}

					if (type == EJson::Boolean)
					{
						bool value = abstractValue->AsBool();
						modularObject->SetParameterValue(valueName, value);
					}

					if (type == EJson::Number)
					{
						double value = abstractValue->AsNumber();
						modularObject->SetParameterValue(valueName, value);
					}
				}
			}
		}
	}

	return isDeserializeSuccessful;
}

FString UPresetHandler::GetFullDirectory()
{
	FString persistentDir = FPlatformProcess::UserSettingsDir();
	FString fullDir = persistentDir + m_subFolders + "/";
	return fullDir;
}

bool UPresetHandler::SavePresetsToFile()
{
	TSharedPtr<FJsonObject> jsonWriteObject = MakeShareable(new FJsonObject);

	for (TPair<FString, FString> preset : m_presetsAsJson)
	{
		FString name = preset.Key;
		FString json = preset.Value;

		jsonWriteObject->SetStringField(name, json);
	}

	FString jsonString;
	TSharedRef<TJsonWriter<>> JsonWriter = TJsonWriterFactory<>::Create(&jsonString);

	if (!FJsonSerializer::Serialize(jsonWriteObject.ToSharedRef(), JsonWriter))
	{
		return false;
	}

	return SaveStringToFile(jsonString);
}

bool UPresetHandler::LoadPresetsFromFile()
{
	FString jsonString;

	if (!LoadStringFromFile(jsonString))
	{
		return false;
	}

	TSharedPtr<FJsonObject> jsonLoadObject = MakeShareable(new FJsonObject);
	TSharedRef<TJsonReader<>> jsonReader = TJsonReaderFactory<>::Create(jsonString);

	bool isDeserializeSuccessful = false;

	if (FJsonSerializer::Deserialize(jsonReader, jsonLoadObject))
	{
		m_presetsAsJson.Empty();

		isDeserializeSuccessful = true;

		TMap<FString, TSharedPtr<FJsonValue>> jsonPairs = jsonLoadObject->Values;

		for (TPair<FString, TSharedPtr<FJsonValue>> jsonPair : jsonPairs)
		{
			FString valueName = jsonPair.Key;
			TSharedPtr<FJsonValue> abstractValue = jsonPair.Value;

			if (jsonPair.Value->Type == EJson::String)
			{
				FString value = abstractValue->AsString();
				m_presetsAsJson.Add(valueName, value);
			}
			else
			{
				UDebug::Log(FString::Printf(TEXT("Unexpected type in json object %s"), *jsonPair.Key));
			}
		}
	}

	return isDeserializeSuccessful;
}

bool UPresetHandler::SaveStringToFile(const FString& presetsAsJson)
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

	return FFileHelper::SaveStringToFile(*presetsAsJson, *filePath);
}

bool UPresetHandler::LoadStringFromFile(FString& presetsAsJson)
{
	FString fullDir = GetFullDirectory();
	UDebug::Log(*FString::Printf(TEXT("Reading from directory: %s"), *fullDir));

	IFileManager& fileManager = IFileManager::Get();

	if (!fileManager.DirectoryExists(*fullDir))
	{
		UDebug::Warning(*FString::Printf(TEXT("Directory '%s' does not exist"), *fullDir));

		return false;
	}

	FString filePath = fullDir + m_filePath;

	if (!fileManager.FileExists(*filePath))
	{
		UDebug::Warning(*FString::Printf(TEXT("File '%s' does not exist"), *filePath));

		return false;
	}

	bool isSuccessful = FFileHelper::LoadFileToString(presetsAsJson, *filePath);
	return isSuccessful;
}

TArray<FString> UPresetHandler::GetPresetNames() const
{
	TArray<FString> presetNames;
	
	for (TPair<FString, FString> preset : m_presetsAsJson)
	{
		presetNames.Add(preset.Key);
	}

	return presetNames;
}