// Fill out your copyright notice in the Description page of Project Settings.


#include "Formula1Api.h"
#include "../Debug/Debug.h"
#include "../Utilities/Utility.h"
#include "../SportData/Team.h"
#include "../SportData/SportPlayer.h"
#include "../SportData/F1Driver.h"
#include "../Core/CoreSystem.h"
#include "../EventSystem/EventSystem.h"
#include "../Debug/Debug.h"
#include "Formula1ApiThreadHelper.h"

UFormula1Api::UFormula1Api()
{
	m_isShuttingDown = false;

	m_isCreatedOnRunning = GIsRunning;
}

UFormula1Api::~UFormula1Api()
{
	m_isShuttingDown = true;

	if (m_threadHelper != nullptr)
	{
		while (m_threadHelper->IsRunning())
		{
		}
		m_thread->Kill(true);
	}

	m_drivers.Empty();
	m_constructors.Empty();

	if (m_threadHelper != nullptr)
	{
		delete m_threadHelper;
	}
}

void UFormula1Api::Init(FHttpModule& newHttpModule)
{
	m_httpModule = &newHttpModule;

	UDebug::Log("Creating thread helper");
	m_threadHelper = new FFormula1ApiThreadHelper(*this);

	m_constructors = TArray<ConstructorData>();
	m_drivers = TArray<DriverData>();
}

void UFormula1Api::Tick(float deltaTime)
{
	if (m_sendApiDataEvent)
	{
		if (UCoreSystem::Exists())
		{
			UEventSystem* const eventSystem = UCoreSystem::Get().GetEventSystem();
			if (eventSystem != nullptr)
			{
				eventSystem->CallApiDataLoadedEvent(Sport::Formula1);
			}
		}

		m_sendApiDataEvent = false;
	}
}

bool UFormula1Api::IsTickable() const
{
	return m_isCreatedOnRunning;
}

TStatId UFormula1Api::GetStatId() const
{
	return UObject::GetStatID();
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

	if (m_isShuttingDown)
	{
		return;
	}

	TSharedRef<IHttpRequest, ESPMode::ThreadSafe> request = m_httpModule->CreateRequest();
	request->OnProcessRequestComplete().BindUObject(this, &UFormula1Api::ConstructorDataCallback);

	//process GET request
	request->SetURL("http://ergast.com/api/f1/2021/constructorStandings.json");
	request->SetVerb("GET");
	request->SetHeader(TEXT("User-Agent"), "X-UnrealEngine-Agent");
	request->SetHeader("Content-Type", TEXT("application/json"));
	request->ProcessRequest();
}

void UFormula1Api::ConstructorDataCallback(FHttpRequestPtr request, FHttpResponsePtr response, bool isSuccessful)
{
	UDebug::Log("PullConstructorsData callback");

	if (!isSuccessful)
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
}

void UFormula1Api::PullDriverChampionship()
{
	if (m_isShuttingDown)
	{
		return;
	}

	m_criticalSection.Lock();
	m_driverChampionshipResponse.isFinished = false;
	m_driverChampionshipResponse.isSuccessful = false;
	m_criticalSection.Unlock();

	if (m_isShuttingDown)
	{
		return;
	}

	TSharedRef<IHttpRequest, ESPMode::ThreadSafe> Request = m_httpModule->CreateRequest();
	Request->OnProcessRequestComplete().BindUObject(this, &UFormula1Api::DriverChampionShipCallback);

	//process GET request
	Request->SetURL("http://ergast.com/api/f1/2021/driverstandings.json");
	Request->SetVerb("GET");
	Request->SetHeader(TEXT("User-Agent"), "X-UnrealEngine-Agent");
	Request->SetHeader("Content-Type", TEXT("application/json"));
	Request->ProcessRequest();
}

void UFormula1Api::DriverChampionShipCallback(FHttpRequestPtr request, FHttpResponsePtr response, bool isSuccessful)
{
	UDebug::Log("PullDriverChampionship callback");

	if (!isSuccessful)
	{
		UDebug::Error("PullDriverChampionship was unsuccessful");
		UDebug::ToScreen("Error: PullDriverChampionship was unsuccessful");

		m_criticalSection.Lock();
		m_driverChampionshipResponse.isFinished = true;
		m_driverChampionshipResponse.isSuccessful = false;
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

			TSharedPtr<FJsonObject> standingsListObject = JsonObject->GetObjectField("MRData")->GetObjectField("StandingsTable")->GetArrayField("StandingsLists")[0]->AsObject();
			TSharedPtr<FJsonObject> standingsObject = standingsListObject->GetArrayField("DriverStandings")[i]->AsObject();
			TSharedPtr<FJsonObject> driverObject = standingsObject->GetObjectField("Driver");
			TSharedPtr<FJsonObject> constructorObject = standingsObject->GetArrayField("Constructors")[0]->AsObject();

			double points = standingsObject->GetNumberField("points");
			double wins = standingsObject->GetNumberField("wins");
			double position = standingsObject->GetNumberField("position");

			FString driverId = driverObject->GetStringField("driverId");
			FString firstName = driverObject->GetStringField("givenName");
			FString lastName = driverObject->GetStringField("familyName");
			FString codeName = driverObject->GetStringField("code");
			double number = driverObject->GetNumberField("permanentNumber");
			FString nationality = driverObject->GetStringField("nationality");
			FString dob = driverObject->GetStringField("dateOfBirth");

			FString constructorId = constructorObject->GetStringField("constructorId");
			FString constructorName = constructorObject->GetStringField("name");

			DriverData driver
			{
				driverId,
				firstName,
				lastName,
				codeName,
				static_cast<int>(number),
				nationality,
				constructorName,
				dob,
				static_cast<float>(points),
				static_cast<int>(wins),
			};

			m_criticalSection.Lock();
			m_drivers.Add(driver);
			m_criticalSection.Unlock();
		}

		m_criticalSection.Lock();
		m_driverChampionshipResponse.isFinished = true;
		m_driverChampionshipResponse.isSuccessful = true;
		m_criticalSection.Unlock();
	}
	else
	{
		UDebug::Error("API deserialization unsuccessful");
		UDebug::ToScreen("Error: API deserialization unsuccessful");

		m_criticalSection.Lock();
		m_driverChampionshipResponse.isFinished = true;
		m_driverChampionshipResponse.isSuccessful = false;
		m_criticalSection.Unlock();
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
			UF1Driver& f1Driver = UF1Driver::Make(driver.firstName, driver.lastName, driver.codeName, *team);
			f1Driver.nationalityAsString = driver.nationality;
			f1Driver.SetNumber(driver.number);
			f1Driver.SetDateOfBirth(UnpackDateOfBirthString(driver));
			f1Driver.championshipPoints = driver.points;
			f1Driver.wins = driver.wins;

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

bool UFormula1Api::IsDriverChampionshipPulled(bool& wasSuccessful)
{
	bool isFinished = false;

	m_criticalSection.Lock();
	isFinished = m_driverChampionshipResponse.isFinished;
	wasSuccessful = m_driverChampionshipResponse.isSuccessful;
	m_criticalSection.Unlock();

	return isFinished || m_isShuttingDown;
}

void UFormula1Api::SendApiDataEvent()
{
	m_sendApiDataEvent = true;
}

FDateTime UFormula1Api::UnpackDateOfBirthString(const DriverData& driver)
{
	const static FString format = "YYYY-MM-DD";

	FString dob = driver.dob;
		
	int yearSeperatorIndex;
	int monthSeperatorIndex;

	if (!dob.FindChar('-', yearSeperatorIndex))
	{
		UDebug::Warning(FString::Printf(TEXT("Birthday of driver %s is invalid (not %s)"), *driver.driverId, *format));
		return FDateTime();
	}

	FString yearString = dob.Mid(0, yearSeperatorIndex);

	FString monthDayString = dob.Mid(yearSeperatorIndex + 1, dob.Len() - yearSeperatorIndex - 1);

	if (!monthDayString.FindChar('-', monthSeperatorIndex))
	{
		UDebug::Warning(FString::Printf(TEXT("Birthday of driver %s is invalid (not %s)"), *driver.driverId, *format));
		return FDateTime();
	}

	FString monthString = monthDayString.Mid(0, monthSeperatorIndex);

	FString dayString = monthDayString.Mid(monthSeperatorIndex + 1, monthDayString.Len() - monthSeperatorIndex - 1);

	int year = FCString::Atoi(*yearString);
	int month = FCString::Atoi(*monthString);
	int day = FCString::Atoi(*dayString);

	return FDateTime(year, month, day);
}