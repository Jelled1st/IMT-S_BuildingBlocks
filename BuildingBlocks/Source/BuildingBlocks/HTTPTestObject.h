// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Runtime/Online/HTTP/Public/Http.h"
#include "UObject/NoExportTypes.h"
#include "HTTPTestObject.generated.h"

/**
 * 
 */
UCLASS()
class BUILDINGBLOCKS_API UHTTPTestObject : public UObject
{
	GENERATED_BODY()

public:

	UHTTPTestObject();
	void Start();

	FHttpModule* httpModule;


	UFUNCTION()
		//TODO: add bindable object and url to get request in order to minimize amount of get requests
		void HttpGetRequest();
		void HttpGetRacersOf2020();
		void HttpGetNationalitiesOf2020();
		void HttpGetRaceTracksOf2020();
		void HttpGetSponsorsOf2021();


	//TODO: condeness responses in one unqie response based on url?
	void OnResponseReceived(FHttpRequestPtr request, FHttpResponsePtr response, bool isSuccessful);
	void OnResponseReceivedRacers(FHttpRequestPtr request, FHttpResponsePtr response, bool isSuccessful);
	void OnResponseReceivedNationality(FHttpRequestPtr request, FHttpResponsePtr response, bool isSuccessful);
	void OnResponseReceivedTracks(FHttpRequestPtr request, FHttpResponsePtr response, bool isSuccessful);
	void OnResponseReceivedSponsors(FHttpRequestPtr request, FHttpResponsePtr response, bool isSuccessful);

	TArray<FString> GetTeamNames();
	TArray<FString> GetTeamScore();
	TArray<FString> GetTeamNationalities();
	float GetTeamScore(FString team);
	FString GetTeamNationality(FString team);


	//these are to be replaced by player and team objects
	TArray<FString> racersNameOf2020;
	TArray<FString> nationalitiesOf2020;
	TArray<FString> racerTracksOf2020;

	//2021 SPONSORS
	TArray<FString> f1Teams2021;
	TArray<FString> f1TeamNationalities2021;
	TArray<FString> f1TeamPoints2021;
	TMap<FString, float> f1TeamPointsByTeam2021;
	TMap<FString, FString> f1TeamNationalitiesByTeam2021;

	//TODO public list of players, race tracks, nationalities, teams. To be replaced by players & team classes in fututre iteration
};
