// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "../SportData/Country.h"
#include "../debug/debug.h"
#include "Utility.generated.h"

UCLASS()
class BUILDINGBLOCKS_API UUtility : public UObject
{
	GENERATED_BODY()
	
public:
	static inline char* FStringToCharPtr(const FString& string)
	{
		return TCHAR_TO_ANSI(*string);
	}

	static inline FString CharPtrToFString(const char* const string)
	{
		return FString(string);
	}

	static TArray<Country> CreateCountryArray()
	{
		TArray<Country> countries;
		for (int i = 0; i < Country::Last; ++i)
		{
			countries.Add(static_cast<Country>(i));
		}
		return countries;
	}

	// From: https://forums.unrealengine.com/t/uenum-and-getvalueasstring/23377
	static const FString EnumToString(const TCHAR* enumType, int32 enumValue)
	{
		const UEnum* enumPtr = FindObject<UEnum>(ANY_PACKAGE, enumType, true);

		if (!enumPtr)
		{
			UDebug::Error(FString::Printf(TEXT("Could not concert enum '%s' to string"), enumType));
			return "Invalid";
		}

#if WITH_EDITOR
		return enumPtr->GetDisplayNameTextByIndex(enumValue).ToString();
#else
		return enumPtr->GetEnumName(enumValue);
#endif
	}

	static int FStringToInt(const FString& string)
	{
		// unsafe - no way to detect errors
		return FCString::Atoi(*string);
	}

	static int FStringToFloat(const FString& string)
	{
		// unsafe - no way to detect errors
		return FCString::Atof(*string);
	}
};
