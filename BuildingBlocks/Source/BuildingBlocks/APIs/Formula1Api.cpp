// Fill out your copyright notice in the Description page of Project Settings.


#include "Formula1Api.h"
#include "../Debug/Debug.h"
#include "../Utilities/Utility.h"
#include "../SportData/Team.h"
#include "../SportData/SportPlayer.h"
#include "../Core/CoreSystem.h"
#include "../Debug/Debug.h"
#include "Formula1ApiThreadHelper.h"

UFormula1Api::UFormula1Api()
{
	m_isShuttingDown = false;
}

UFormula1Api::~UFormula1Api()
{
	m_isShuttingDown = true;
	if (m_threadHelper != nullptr)
	{
		while (m_threadHelper->IsRunning())
		{
		}
	}

	m_drivers.Empty();
	m_constructors.Empty();
	m_teamDriversResponse.teamApiResponses.Empty();

	delete m_threadHelper;
}

void UFormula1Api::Init(FHttpModule& newHttpModule)
{
	m_httpModule = &newHttpModule;

	UDebug::Log("Creating thread helper");
	m_threadHelper = new FFormula1ApiThreadHelper(*this);

	m_constructors = TArray<ConstructorData>();
	m_drivers = TArray<DriverData>();
}

void UFormula1Api::PullApiDataAsync()
{
	if (!m_threadHelper->IsRunning())
	{
		UDebug::Log("Creating runnable thread");
		m_thread = FRunnableThread::Create(m_threadHelper, TEXT("Formula1Api_Thread"));
	}
}

void UFormula1Api::PullConstructorsData()
{
	if (m_isShuttingDown)
	{
		return;
	}

	m_criticalSection.Lock();
	m_constructors.Empty();

	m_constructorsResponse.isFinished = false;
	m_constructorsResponse.isSuccessful = false;
	m_criticalSection.Unlock();

	static auto callback = [this](FHttpRequestPtr request, FHttpResponsePtr response, bool isResponseSuccessful)
	{
		if (m_isShuttingDown)
		{
			return;
		}

		if (!isResponseSuccessful)
		{
			UDebug::Error("PullTeamsData was unsuccessful");
			UDebug::ToScreen("Error: PullTeamsData was unsuccessful");

			m_criticalSection.Lock();
			m_constructorsResponse.isFinished = true;
			m_constructorsResponse.isSuccessful = false;
			m_criticalSection.Unlock();

			return;
		}

		TSharedPtr<FJsonObject> JsonObject;
		TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(response->GetContentAsString());

		if (FJsonSerializer::Deserialize(Reader, JsonObject))
		{
			FString constructorCountString = JsonObject->GetObjectField("MRData")->GetStringField("total");
			int32 constructorCount = UUtility::FStringToInt(constructorCountString);

			for (int i = 0; i < constructorCount; i++) 
			{
				if (m_isShuttingDown)
				{
					return;
				}

				TSharedPtr<FJsonObject> constructor = JsonObject->GetObjectField("MRData")->GetObjectField("StandingsTable")->GetArrayField("StandingsLists")[0]->AsObject()->GetArrayField("ConstructorStandings")[i]->AsObject();

				FString constructorId = constructor->GetObjectField("Constructor")->GetStringField("constructorId");
				FString constructorName = constructor->GetObjectField("Constructor")->GetStringField("name");
				FString nationality = constructor->GetObjectField("Constructor")->GetStringField("nationality");
				double points = constructor->GetNumberField("points");
				double wins = constructor->GetNumberField("wins");

				ConstructorData newTeam
				{
					constructorId,
					constructorName,
					nationality,
					static_cast<float>(points),
					static_cast<int>(wins),
					"Unknown",
					"Unknown",
				};

				m_criticalSection.Lock();
				m_constructors.Add(newTeam);
				m_criticalSection.Unlock();
			}

			m_criticalSection.Lock();
			m_constructorsResponse.isFinished = true;
			m_constructorsResponse.isSuccessful = true;
			m_criticalSection.Unlock();
		}
		else
		{
			UDebug::Error("API deserialization unsuccessful");
			UDebug::ToScreen("Error: API deserialization unsuccessful");

			m_criticalSection.Lock();
			m_constructorsResponse.isFinished = true;
			m_constructorsResponse.isSuccessful = false;
			m_criticalSection.Unlock();
		}
	};

	if (m_isShuttingDown)
	{
		return;
	}

	TSharedRef<IHttpRequest, ESPMode::ThreadSafe> Request = m_httpModule->CreateRequest();
	Request->OnProcessRequestComplete().BindLambda(callback);

	//process GET request
	Request->SetURL("http://ergast.com/api/f1/2021/constructorStandings.json");
	Request->SetVerb("GET");
	Request->SetHeader(TEXT("User-Agent"), "X-UnrealEngine-Agent");
	Request->SetHeader("Content-Type", TEXT("application/json"));
	Request->ProcessRequest();
}

void UFormula1Api::PullDriverInformation()
{
	if (m_isShuttingDown)
	{
		return;
	}

	m_criticalSection.Lock();
	m_drivers.Empty();

	m_driversResponse.isFinished = false;
	m_driversResponse.isSuccessful = false;
	m_criticalSection.Unlock();

	auto callback = [this](FHttpRequestPtr request, FHttpResponsePtr response, bool isSuccessful)
	{
		UDebug::Log("PullTeamsData callback");
		if (m_isShuttingDown)
		{
			return;
		}

		if (!isSuccessful)
		{
			UDebug::Error("PullTeamsData was unsuccessful");
			UDebug::ToScreen("Error: PullTeamsData was unsuccessful");

			m_criticalSection.Lock();
			m_driversResponse.isFinished = true;
			m_driversResponse.isSuccessful = false;
			m_criticalSection.Unlock();

			return;
		}

		TSharedPtr<FJsonObject> JsonObject;
		TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(response->GetContentAsString());

		if (FJsonSerializer::Deserialize(Reader, JsonObject))
		{
			FString driverCountString = JsonObject->GetObjectField("MRData")->GetStringField("total");
			int32 driverCount = UUtility::FStringToInt(driverCountString);

			for (int i = 0; i < driverCount; i++) 
			{
				if (m_isShuttingDown)
				{
					return;
				}

				TSharedPtr<FJsonObject> driverObject = JsonObject->GetObjectField("MRData")->GetObjectField("DriverTable")->GetArrayField("Drivers")[i]->AsObject();

				FString driverId = driverObject->GetStringField("driverId");
				FString firstName = driverObject->GetStringField("givenName");
				FString lastName = driverObject->GetStringField("familyName");
				FString codeName = driverObject->GetStringField("code");
				double number = driverObject->GetNumberField("permanentNumber");
				FString nationality = driverObject->GetStringField("nationality");

				DriverData driver
				{
					driverId,
					firstName,
					lastName,
					codeName,
					static_cast<int>(number),
					nationality,
					"Unknown"
				};

				m_criticalSection.Lock();
				m_drivers.Add(driver);
				m_criticalSection.Unlock();
			}

			m_criticalSection.Lock();
			m_driversResponse.isFinished = true;
			m_driversResponse.isSuccessful = true;
			m_criticalSection.Unlock();
		}
		else
		{
			UDebug::Error("API deserialization unsuccessful");
			UDebug::ToScreen("Error: API deserialization unsuccessful");

			m_criticalSection.Lock();
			m_driversResponse.isFinished = true;
			m_driversResponse.isSuccessful = false;
			m_criticalSection.Unlock();
		}
	};

	if (m_isShuttingDown)
	{
		return;
	}

	TSharedRef<IHttpRequest, ESPMode::ThreadSafe> Request = m_httpModule->CreateRequest();
	Request->OnProcessRequestComplete().BindLambda(callback);

	//process GET request
	Request->SetURL("http://ergast.com/api/f1/2021/drivers.json");
	Request->SetVerb("GET");
	Request->SetHeader(TEXT("User-Agent"), "X-UnrealEngine-Agent");
	Request->SetHeader("Content-Type", TEXT("application/json"));
	Request->ProcessRequest();
}

void UFormula1Api::PullTeamDrivers()
{
	if (m_isShuttingDown)
	{
		return;
	}

	m_criticalSection.Lock();
	m_teamDriversResponse.expectedResponses = m_constructors.Num();
	m_teamDriversResponse.teamApiResponses.Empty();
	m_criticalSection.Unlock();

	int constructorIndex = 0;
	for (ConstructorData& constructor : m_constructors)
	{
		if (m_isShuttingDown)
		{
			return;
		}

		auto callback = [this, &constructor, constructorIndex](FHttpRequestPtr request, FHttpResponsePtr response, bool isSuccessful)
		{
			if (m_isShuttingDown)
			{
				return;
			}

			if (!isSuccessful)
			{
				UDebug::Error("PullTeamsData was unsuccessful");
				UDebug::ToScreen("Error: PullTeamsData was unsuccessful");

				m_criticalSection.Lock();
				m_teamDriversResponse.teamApiResponses.Add(ResponseData{true, false});
				m_criticalSection.Unlock();

				return;
			}

			TSharedPtr<FJsonObject> JsonObject;
			TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(response->GetContentAsString());

			if (FJsonSerializer::Deserialize(Reader, JsonObject))
			{
				TSharedPtr<FJsonObject> driverTable = JsonObject->GetObjectField("MRData")->GetObjectField("DriverTable");
				
				 FString driverId1 = driverTable->GetArrayField("Drivers")[0]->AsObject()->GetStringField("driverId");
				 FString driverId2 = driverTable->GetArrayField("Drivers")[1]->AsObject()->GetStringField("driverId");

				 constructor.driver1Id = driverId1;
				 constructor.driver2Id = driverId2;

				 DriverData* driver1 = FindDriver(driverId1, NameFilter::DriverId);
				 DriverData* driver2 = FindDriver(driverId2, NameFilter::DriverId);

				 m_criticalSection.Lock();
				 if (driver1 != nullptr)
				 {
					 driver1->constructorName = constructor.name;
				 }
				 if (driver2 != nullptr)
				 {
					 driver2->constructorName = constructor.name;
				 }

				 m_teamDriversResponse.teamApiResponses.Add(ResponseData{ true, true });

				 m_criticalSection.Unlock();
			}
			else
			{
				UDebug::Error("API deserialization unsuccessful");
				UDebug::ToScreen("Error: API deserialization unsuccessful");

				m_criticalSection.Lock();
				m_teamDriversResponse.teamApiResponses.Add(ResponseData{ true, false });
				m_criticalSection.Unlock();
			}
		};

		if (m_isShuttingDown)
		{
			return;
		}
		TSharedRef<IHttpRequest, ESPMode::ThreadSafe> Request = m_httpModule->CreateRequest();
		Request->OnProcessRequestComplete().BindLambda(callback);

		FString apiUrl = "http://ergast.com/api/f1/2021/last/constructors/" + constructor.teamId + "/drivers.json";

		Request->SetURL(apiUrl);
		Request->SetVerb("GET");
		Request->SetHeader(TEXT("User-Agent"), "X-UnrealEngine-Agent");
		Request->SetHeader("Content-Type", TEXT("application/json"));
		Request->ProcessRequest();

		++constructorIndex;
	}
}

void UFormula1Api::ImportDataToSportHandler()
{
	USportDataHandler& sportHandler = UCoreSystem::Get().GetSportDataHandler();

	m_criticalSection.Lock();
	for (ConstructorData& constructor : m_constructors)
	{
		UTeam& team = UTeam::Make(constructor.name, Sport::Formula1, constructor.points, constructor.nationality);
		team.wins = constructor.wins;
	}

	for (DriverData& driver : m_drivers)
	{
		UTeam* team = sportHandler.FindTeam(driver.constructorName, Sport::Formula1);

		if (team != nullptr)
		{
			USportPlayer& player = USportPlayer::Make(driver.firstName, driver.lastName, driver.codeName, *team);
			player.nationalityAsString = driver.nationality;
		}
		else
		{
			UDebug::Error(FString::Printf(TEXT("Could not find a valid team for driver %s %s (%s) in team %s"), *driver.firstName, *driver.lastName, *driver.codeName, *driver.constructorName));
			UDebug::ToScreen(FString::Printf(TEXT("Could not find a valid team for driver %s %s (%s) in team %s"), *driver.firstName, *driver.lastName, *driver.codeName, *driver.constructorName));
		}
	}
	m_criticalSection.Unlock();
}

UFormula1Api::DriverData* UFormula1Api::FindDriver(FString name, NameFilter filter)
{
	m_criticalSection.Lock();
	for (DriverData& driver : m_drivers)
	{
		bool isDriver = false;

		switch (filter)
		{
			case NameFilter::FirstName:
			{
				isDriver = driver.firstName == name;
				break;
			}
			case NameFilter::LastName:
			{
				isDriver = driver.lastName == name;
				break;
			}
			case NameFilter::CodeName:
			{
				isDriver = driver.codeName == name;
				break;
			}
			case NameFilter::DriverId:
			{
				isDriver = driver.driverId == name;
				break;
			}
		}

		if (isDriver)
		{
			m_criticalSection.Unlock();
			return &driver;
		}
	}
	m_criticalSection.Unlock();
	return nullptr;
}

bool UFormula1Api::IsContructorDataPulled(bool& wasSuccessful)
{
	bool isFinished = false;

	m_criticalSection.Lock();
	isFinished = m_constructorsResponse.isFinished;
	wasSuccessful = m_constructorsResponse.isSuccessful;
	m_criticalSection.Unlock();

	return isFinished || m_isShuttingDown;
}

bool UFormula1Api::IsDriversInfoPulled(bool& wasSuccessful)
{
	bool isFinished = false;

	m_criticalSection.Lock();
	isFinished = m_driversResponse.isFinished;
	wasSuccessful = m_driversResponse.isSuccessful;
	m_criticalSection.Unlock();

	return isFinished || m_isShuttingDown;
}

bool UFormula1Api::IsTeamDriversPulled(bool& wasSuccessful)
{
	bool isFinished = true;
	wasSuccessful = true;

	m_criticalSection.Lock();

	if (m_teamDriversResponse.expectedResponses == m_teamDriversResponse.teamApiResponses.Num())
	{
		for (ResponseData response : m_teamDriversResponse.teamApiResponses)
		{
			if (!response.isFinished)
			{
				isFinished = false;
			}
			if (!response.isSuccessful)
			{
				wasSuccessful = false;
			}
		}
	}
	else
	{
		isFinished = false;
		wasSuccessful = false;
	}

	m_criticalSection.Unlock();

	return isFinished || m_isShuttingDown;
}
