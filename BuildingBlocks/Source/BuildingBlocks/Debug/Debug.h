// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#define DebugLog(msg, ...)		UE_LOG(LogTemp, Log, TEXT(msg), ##__VA_ARGS__)
#define DebugError(msg, ...)	UE_LOG(LogTemp, Error, TEXT(msg), ##__VA_ARGS__)
#define DebugWarning(msg, ...)	UE_LOG(LogTemp, Warning, TEXT(msg), ##__VA_ARGS__)

class BUILDINGBLOCKS_API Debug
{
public:
	Debug();
	~Debug();
};
