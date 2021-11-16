// Fill out your copyright notice in the Description page of Project Settings.


#include "Formula1Api.h"
#include "../Debug/Debug.h"
#include "../Utilities/Utility.h"
#include "../SportData/Team.h"

UFormula1Api::UFormula1Api()
{

}

void UFormula1Api::Init(FHttpModule& newHttpModule)
{
	m_httpModule = &newHttpModule;
}

void UFormula1Api::PullApiData(bool applyDataToSportHandler)
{
	PullTeamsData(applyDataToSportHandler);
}

void UFormula1Api::PullTeamsData(bool applyDataToSportHandler)
{
	auto response = [this, applyDataToSportHandler](FHttpRequestPtr request, FHttpResponsePtr response, bool isSuccessful)
	{
		if (!isSuccessful)
		{
			UDebug::Error("PullTeamsData was unsuccessful");
			UDebug::ToScreen("Error: PullTeamsData was unsuccessful");
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
				TSharedPtr<FJsonObject> construcorStandings = JsonObject->GetObjectField("MRData")->GetObjectField("StandingsTable")->GetArrayField("StandingsLists")[0]->AsObject()->GetArrayField("ConstructorStandings")[i]->AsObject();

				FString constructor = construcorStandings->GetObjectField("Constructor")->GetStringField("name");
				FString nationality = construcorStandings->GetObjectField("Constructor")->GetStringField("nationality");
				FString points = construcorStandings->GetStringField("points");
				FString wins = construcorStandings->GetStringField("wins");

				TeamData newTeam
				{
					constructor,
					nationality,
					UUtility::FStringToFloat(points),
					UUtility::FStringToInt(wins),
				};

				m_teams.Add(newTeam);
			}

			if (applyDataToSportHandler)
			{
				ImportDataToSportHandler();
			}
		}
		else
		{
			UDebug::Error("API deserialization unsuccessful");
			UDebug::ToScreen("Error: API deserialization unsuccessful");
		}
	};

	TSharedRef<IHttpRequest, ESPMode::ThreadSafe> Request = m_httpModule->CreateRequest();
	Request->OnProcessRequestComplete().BindLambda(response);

	//process GET request
	Request->SetURL("http://ergast.com/api/f1/2021/constructorStandings.json");
	Request->SetVerb("GET");
	Request->SetHeader(TEXT("User-Agent"), "X-UnrealEngine-Agent");
	Request->SetHeader("Content-Type", TEXT("application/json"));
	Request->ProcessRequest();
}

void UFormula1Api::PullPlayerData()
{

}

void UFormula1Api::ImportDataToSportHandler()
{
	for (TeamData& team : m_teams)
	{
		UTeam& uTeam = UTeam::Make(team.name, Sport::Formula1, team.points, team.nationality);
		uTeam.wins = team.wins;
	}
}
