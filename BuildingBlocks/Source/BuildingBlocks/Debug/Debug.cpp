// Fill out your copyright notice in the Description page of Project Settings.


#include "Debug.h"

Debug::Debug()
{
}

Debug::~Debug()
{
}

void Debug::Log(FString msg)
{
	UE_LOG(LogTemp, Log, TEXT("%s"), *msg);
}

void Debug::Error(FString msg)
{
	UE_LOG(LogTemp, Error, TEXT("%s"), *msg);
}

void Debug::Warning(FString msg)
{
	UE_LOG(LogTemp, Warning, TEXT("%s"), *msg);
}

void Debug::ToScreen(FString msg, FColor color, float time, int32 key, bool newerOnTop, FVector2D scale)
{
	GEngine->AddOnScreenDebugMessage(key, time, color, msg, newerOnTop, scale);
}
