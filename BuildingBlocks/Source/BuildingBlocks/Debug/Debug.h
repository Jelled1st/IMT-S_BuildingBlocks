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

	static void Log(FString msg);

	static void Error(FString msg);

	static void Warning(FString msg);

	static void ToScreen(FString msg, FColor color = FColor::White, float time = 5.0f, int32 key = -1, bool newerOnTop = true, FVector2D scale = FVector2D::UnitVector);
};
