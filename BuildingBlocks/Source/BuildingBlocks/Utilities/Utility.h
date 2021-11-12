// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
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
};
