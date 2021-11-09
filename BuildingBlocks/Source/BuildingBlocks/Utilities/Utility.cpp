// Fill out your copyright notice in the Description page of Project Settings.


#include "Utility.h"

char* UUtility::FStringToCharPtr(const FString& string)
{
	return TCHAR_TO_ANSI(*string);
}
