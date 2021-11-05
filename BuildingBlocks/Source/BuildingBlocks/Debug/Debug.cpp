// Fill out your copyright notice in the Description page of Project Settings.


#include "Debug.h"

UDebug::UDebug()
{
}

UDebug::~UDebug()
{
}

void UDebug::Log(FString msg)
{
	UE_LOG(LogTemp, Log, TEXT("%s"), *msg);
}

void UDebug::Log(FString msg, FVector vector)
{
	UE_LOG(LogTemp, Log, TEXT("%s(%f;%f;%f)"), *msg, vector.X, vector.Y, vector.Z);
}

void UDebug::Error(FString msg)
{
	UE_LOG(LogTemp, Error, TEXT("%s"), *msg);
}

void UDebug::Warning(FString msg)
{
	UE_LOG(LogTemp, Warning, TEXT("%s"), *msg);
}

void UDebug::ToScreen(FString msg, FColor color, float time, int32 key, bool newerOnTop, FVector2D scale)
{
	GEngine->AddOnScreenDebugMessage(key, time, color, msg, newerOnTop, scale);
}
