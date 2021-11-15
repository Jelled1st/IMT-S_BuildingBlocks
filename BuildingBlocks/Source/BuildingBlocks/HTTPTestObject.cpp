// Fill out your copyright notice in the Description page of Project Settings.


#include "HTTPTestObject.h"
#include "Kismet/KismetStringLibrary.h"
#include "Runtime/Engine/Public/EngineGlobals.h"
#include "Runtime/Online/HTTP/Public/Http.h"


UHTTPTestObject::UHTTPTestObject() {

	httpModule = &FHttpModule::Get();
}
void UHTTPTestObject::Start()
{
	//TODO: public callable htttp get request
	//HttpGetRequest();
	//HttpGetRacersOf2020();
	//HttpGetNationalitiesOf2020();
	//HttpGetRaceTracksOf2020();
	HttpGetSponsorsOf2021();

	
}

void UHTTPTestObject::HttpGetRequest()
{
	TSharedRef<IHttpRequest, ESPMode::ThreadSafe> Request = httpModule->CreateRequest();
	Request->OnProcessRequestComplete().BindUObject(this, &UHTTPTestObject::OnResponseReceived);

	//process GET request
	Request->SetURL("https://ergast.com/api/f1/1988.json");
	Request->SetVerb("GET");
	Request->SetHeader(TEXT("User-Agent"), "X-UnrealEngine-Agent");
	Request->SetHeader("Content-Type", TEXT("application/json"));
	Request->ProcessRequest();
}

void UHTTPTestObject::HttpGetRacersOf2020()
{
	TSharedRef<IHttpRequest, ESPMode::ThreadSafe> Request = httpModule->CreateRequest();
	Request->OnProcessRequestComplete().BindUObject(this, &UHTTPTestObject::OnResponseReceivedRacers);

	//process GET request
	Request->SetURL("http://ergast.com/api/f1/2020/drivers.json");
	Request->SetVerb("GET");
	Request->SetHeader(TEXT("User-Agent"), "X-UnrealEngine-Agent");
	Request->SetHeader("Content-Type", TEXT("application/json"));
	Request->ProcessRequest();
}

void UHTTPTestObject::HttpGetNationalitiesOf2020()
{
	TSharedRef<IHttpRequest, ESPMode::ThreadSafe> Request = httpModule->CreateRequest();
	Request->OnProcessRequestComplete().BindUObject(this, &UHTTPTestObject::OnResponseReceivedNationality);

	//process GET request
	Request->SetURL("http://ergast.com/api/f1/2020/drivers.json");
	Request->SetVerb("GET");
	Request->SetHeader(TEXT("User-Agent"), "X-UnrealEngine-Agent");
	Request->SetHeader("Content-Type", TEXT("application/json"));
	Request->ProcessRequest();
}

void UHTTPTestObject::HttpGetRaceTracksOf2020()
{
	TSharedRef<IHttpRequest, ESPMode::ThreadSafe> Request = httpModule->CreateRequest();
	Request->OnProcessRequestComplete().BindUObject(this, &UHTTPTestObject::OnResponseReceivedTracks);

	//process GET request
	Request->SetURL("http://ergast.com/api/f1/2020/circuits.json");
	Request->SetVerb("GET");
	Request->SetHeader(TEXT("User-Agent"), "X-UnrealEngine-Agent");
	Request->SetHeader("Content-Type", TEXT("application/json"));
	Request->ProcessRequest();
}

void UHTTPTestObject::HttpGetSponsorsOf2021()
{
	TSharedRef<IHttpRequest, ESPMode::ThreadSafe> Request = httpModule->CreateRequest();
	Request->OnProcessRequestComplete().BindUObject(this, &UHTTPTestObject::OnResponseReceivedSponsors);

	//process GET request
	Request->SetURL("http://ergast.com/api/f1/2021/constructorStandings.json");
	Request->SetVerb("GET");
	Request->SetHeader(TEXT("User-Agent"), "X-UnrealEngine-Agent");
	Request->SetHeader("Content-Type", TEXT("application/json"));
	Request->ProcessRequest();
}

void UHTTPTestObject::OnResponseReceived(FHttpRequestPtr request, FHttpResponsePtr response, bool isSuccessful)
{
	//Create a pointer to hold the json serialized data
	TSharedPtr<FJsonObject> JsonObject;

	//Create a reader pointer to read the json data
	TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(response->GetContentAsString());

	//Deserialize the json data given Reader and the actual object to deserialize
	if (FJsonSerializer::Deserialize(Reader, JsonObject))
	{
		//Get the value of the json object by field name
		FString raceName = "raceName";

		//get race name from race array with idex 0
		FString recievedString = JsonObject->GetObjectField("MRData")->GetObjectField("RaceTable")->GetArrayField("Races")[0]->AsObject()->GetStringField(raceName);

		//get the number of races array
		int32 recievedInt = JsonObject->GetObjectField("MRData")->GetObjectField("RaceTable")->GetArrayField("Races").Num();

		//TODO: integrate with IMgui
		GEngine->AddOnScreenDebugMessage(1, 2.0f, FColor::Green, recievedString);
		//GEngine->AddOnScreenDebugMessage(1, 2.0f, FColor::Green, FString::FromInt(recievedInt));
	}
}

void UHTTPTestObject::OnResponseReceivedRacers(FHttpRequestPtr request, FHttpResponsePtr response, bool isSuccessful)
{	
	TSharedPtr<FJsonObject> JsonObject;
	TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(response->GetContentAsString());

	if (FJsonSerializer::Deserialize(Reader, JsonObject))
	{

		FString stringOfRacers= JsonObject->GetObjectField("MRData")->GetStringField("total");
		int32 nrOfRacers = UKismetStringLibrary::Conv_StringToInt(stringOfRacers);

		for (int i = 0; i < nrOfRacers; i++) {
			FString firstName = JsonObject->GetObjectField("MRData")->GetObjectField("DriverTable")->GetArrayField("Drivers")[i]->AsObject()->GetStringField("givenName");
			FString lastName = JsonObject->GetObjectField("MRData")->GetObjectField("DriverTable")->GetArrayField("Drivers")[i]->AsObject()->GetStringField("familyName");
			FString fullName = firstName + "_" + lastName;
			racersNameOf2020.Add(fullName);
		}

		//get all names stored in array of racers, if correct it should have the same number as the get request. As of now verifiable only in postman
		GEngine->AddOnScreenDebugMessage(1, 2.0f, FColor::Green, FString::FromInt( racersNameOf2020.Num()));
	}
}

void UHTTPTestObject::OnResponseReceivedNationality(FHttpRequestPtr request, FHttpResponsePtr response, bool isSuccessful)
{
	TSharedPtr<FJsonObject> JsonObject;
	TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(response->GetContentAsString());

	if (FJsonSerializer::Deserialize(Reader, JsonObject))
	{

		FString stringOfRacers = JsonObject->GetObjectField("MRData")->GetStringField("total");
		int32 nrOfRacers = UKismetStringLibrary::Conv_StringToInt(stringOfRacers);

		for (int i = 0; i < nrOfRacers; i++) {
			FString nationality= JsonObject->GetObjectField("MRData")->GetObjectField("DriverTable")->GetArrayField("Drivers")[i]->AsObject()->GetStringField("nationality");
			nationalitiesOf2020.Add(nationality);
		}

		//get all nationalities stored in array of nationalities, if correct it should have the same number as the get request. As of now verifiable only in postman
		GEngine->AddOnScreenDebugMessage(1, 2.0f, FColor::Green, FString::FromInt(nationalitiesOf2020.Num()));
	}
}

void UHTTPTestObject::OnResponseReceivedTracks(FHttpRequestPtr request, FHttpResponsePtr response, bool isSuccessful)
{
	TSharedPtr<FJsonObject> JsonObject;
	TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(response->GetContentAsString());

	if (FJsonSerializer::Deserialize(Reader, JsonObject))
	{

		FString stringOfTracks = JsonObject->GetObjectField("MRData")->GetStringField("total");
		int32 nrOfTracks = UKismetStringLibrary::Conv_StringToInt(stringOfTracks);

		for (int i = 0; i < nrOfTracks; i++) {
			FString nationality = JsonObject->GetObjectField("MRData")->GetObjectField("CircuitTable")->GetArrayField("Circuits")[i]->AsObject()->GetStringField("circuitName");
			racerTracksOf2020.Add(nationality);
		}

		//get all nationalities stored in array of nationalities, if correct it should have the same number as the get request. As of now verifiable only in postman
		GEngine->AddOnScreenDebugMessage(1, 2.0f, FColor::Green, FString::FromInt(racerTracksOf2020.Num()));
	}
}

void UHTTPTestObject::OnResponseReceivedSponsors(FHttpRequestPtr request, FHttpResponsePtr response, bool isSuccessful)
{
	TSharedPtr<FJsonObject> JsonObject;
	TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(response->GetContentAsString());

	if (FJsonSerializer::Deserialize(Reader, JsonObject))
	{

		FString stringOfSponsors = JsonObject->GetObjectField("MRData")->GetStringField("total");
		int32 nrOfSponsors = UKismetStringLibrary::Conv_StringToInt(stringOfSponsors);

		for (int i = 0; i < nrOfSponsors; i++) {
			FString sponsorPoints = JsonObject->GetObjectField("MRData")->GetObjectField("StandingsTable")->GetArrayField("StandingsLists")[0]->AsObject()->GetArrayField("ConstructorStandings")[i]->AsObject()->GetStringField("points");			
			f1TeamPoints2021.Add(sponsorPoints);

			FString sponsorName = JsonObject->GetObjectField("MRData")->GetObjectField("StandingsTable")->GetArrayField("StandingsLists")[0]->AsObject()->GetArrayField("ConstructorStandings")[i]->AsObject()->GetObjectField("Constructor")->GetStringField("name");
			f1Teams2021.Add(sponsorName);

			FString sponsorNationality = JsonObject->GetObjectField("MRData")->GetObjectField("StandingsTable")->GetArrayField("StandingsLists")[0]->AsObject()->GetArrayField("ConstructorStandings")[i]->AsObject()->GetObjectField("Constructor")->GetStringField("nationality");
			f1TeamNationalities2021.Add(sponsorNationality);

			f1TeamPointsByTeam2021.Add(sponsorName, FCString::Atof(*sponsorPoints));
			f1TeamNationalitiesByTeam2021.Add(sponsorName, sponsorNationality);
		}
	}
}

TArray<FString> UHTTPTestObject::GetTeamNames()
{
	return f1Teams2021;
}

TArray<FString> UHTTPTestObject::GetTeamScore()
{
	return f1TeamPoints2021;
}

TArray<FString> UHTTPTestObject::GetTeamNationalities()
{
	return f1TeamNationalities2021;
}

float UHTTPTestObject::GetTeamScore(FString team)
{
	if (!f1TeamPointsByTeam2021.Contains(team))
	{
		return 0;
	}
	else
	{
		return f1TeamPointsByTeam2021[team];
	}
}

FString UHTTPTestObject::GetTeamNationality(FString team)
{
	if (!f1TeamNationalitiesByTeam2021.Contains(team))
	{
		return "Unknown";
	}
	else
	{
		return f1TeamNationalitiesByTeam2021[team];
	}
}
