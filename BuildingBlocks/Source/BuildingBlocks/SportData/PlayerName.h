// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PlayerName.generated.h"

UCLASS(BluePrintType) 
class BUILDINGBLOCKS_API UPlayerName : public UObject
{
	GENERATED_BODY()

public:
	UPlayerName();
	~UPlayerName();

	FString ToString() const;

private:
	UPROPERTY() FString m_firstName;
	UPROPERTY() FString m_lastName;
	UPROPERTY() FString m_displayName;
};
