// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Country.h"
#include "Sport.h"
#include "SportPlayer.generated.h"

class UTeam;

UCLASS(BluePrintType) 
class BUILDINGBLOCKS_API USportPlayer : public UObject
{
	GENERATED_BODY()

public:
	USportPlayer();
	~USportPlayer();

	static USportPlayer& Make(FString firstName, FString lastName, FString displayName, UTeam& team);

	FString GetFullName() const;
	FString GetDisplayName() const;

private:
	UPROPERTY() FString m_firstName;
	UPROPERTY() FString m_lastName;
	UPROPERTY() FString m_displayName;

	UPROPERTY() UTeam* m_team;
	UPROPERTY() int m_age;
	UPROPERTY() int m_length;
	UPROPERTY() float m_weight;
	UPROPERTY() float m_number;
	UPROPERTY() TEnumAsByte<Country> m_nationality;
	UPROPERTY() TEnumAsByte<Sport> m_sport;
};
