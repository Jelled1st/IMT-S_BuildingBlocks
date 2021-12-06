// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Country.h"
#include "Sport.h"
#include "Misc/DateTime.h"
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
	bool Init(FString firstName, FString lastName, FString displayName, UTeam& team);

	FString GetFullName() const;
	FString GetDisplayName() const;
	int GetNumber() const;
	FString GetNumberAsString() const;
	FString GetNationalityAsString() const;

	UPROPERTY() TEnumAsByte<Country> nationality;
	UPROPERTY() FString nationalityAsString;

protected:
	UPROPERTY() FString m_firstName;
	UPROPERTY() FString m_lastName;
	UPROPERTY() FString m_displayName;

	UPROPERTY() UTeam* m_team;
	UPROPERTY() FDateTime m_dateOfBirth;

	UPROPERTY() int m_length;
	UPROPERTY() float m_weight;
	UPROPERTY() float m_number;
	UPROPERTY() TEnumAsByte<Sport> m_sport;
};
