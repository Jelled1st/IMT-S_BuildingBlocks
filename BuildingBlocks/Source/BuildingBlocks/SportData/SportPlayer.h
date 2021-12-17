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

	void SetNumber(int number);
	void SetDateOfBirth(const FDateTime& dob);
	void SetDateOfBirth(int day, int month, int year);

	FString GetFullName() const;
	FString GetDisplayName() const;
	int GetNumber() const;
	FString GetNumberAsString() const;
	FString GetNationalityAsString() const;
	const FDateTime& GetDateOfBirth() const;
	const UTeam* const GetTeam() const;

	UPROPERTY() TEnumAsByte<Country> nationality;
	UPROPERTY() FString nationalityAsString;

protected:
	FString m_firstName;
	FString m_lastName;
	FString m_displayName;

	UPROPERTY();
	UTeam* m_team;

	FDateTime m_dateOfBirth;
	int m_length;
	float m_weight;
	int m_number;
	TEnumAsByte<Sport> m_sport;
};
