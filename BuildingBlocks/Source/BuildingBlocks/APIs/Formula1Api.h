// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Runtime/Online/HTTP/Public/Http.h"
#include "UObject/NoExportTypes.h"
#include "Formula1Api.generated.h"

class FFormula1ApiThreadHelper;

UCLASS()
class BUILDINGBLOCKS_API UFormula1Api : public UObject
{
	GENERATED_BODY()

private:
	enum NameFilter
	{
		FirstName,
		LastName,
		CodeName,
		DriverId,
	};

public:
	struct ConstructorData
	{
	public:
		FString teamId;
		FString name;
		FString nationality;
		float points;
		int wins;
		FString driver1Id;
		FString driver2Id;
	};

	struct DriverData
	{
	public:
		FString driverId;
		FString firstName;
		FString lastName;
		FString codeName;
		int number;
		FString nationality;
		FString constructorName;
	};

	struct ResponseData
	{
	public:
		bool isFinished = false;
		bool isSuccessful = false;
	};

	struct TeamDriversResponseData
	{
		int expectedResponses = 0;
		TArray<ResponseData> teamApiResponses;
	};

	UFormula1Api();
	~UFormula1Api();

	void Init(FHttpModule& newHttpModule);

	void PullApiDataAsync();

	void ImportDataToSportHandler();

	void PullConstructorsData();
	void PullDriverInformation();
	void PullTeamDrivers();

	bool IsContructorDataPulled(bool& wasSuccessful);
	bool IsDriversInfoPulled(bool& wasSuccessful);
	bool IsTeamDriversPulled(bool& wasSuccessful);

private:
	DriverData* FindDriver(FString name, NameFilter filter = NameFilter::FirstName);

	FHttpModule* m_httpModule;

	TArray<ConstructorData> m_constructors;
	TArray<DriverData> m_drivers;

	FRunnableThread* m_thread;
	FFormula1ApiThreadHelper* m_threadHelper;
	FCriticalSection m_criticalSection;

	ResponseData m_constructorsResponse;
	ResponseData m_driversResponse;
	TeamDriversResponseData m_teamDriversResponse;

	bool m_isShuttingDown = false;
};
