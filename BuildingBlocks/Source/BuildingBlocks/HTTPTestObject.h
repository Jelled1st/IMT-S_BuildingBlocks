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


	//TODO: condeness responses in one unqie response based on url?
	void OnResponseReceived(FHttpRequestPtr request, FHttpResponsePtr response, bool isSuccessful);
	void OnResponseReceivedRacers(FHttpRequestPtr request, FHttpResponsePtr response, bool isSuccessful);
	void OnResponseReceivedNationality(FHttpRequestPtr request, FHttpResponsePtr response, bool isSuccessful);
	void OnResponseReceivedTracks(FHttpRequestPtr request, FHttpResponsePtr response, bool isSuccessful);



	//these are to be replaced by player and team objects
	TArray<FString> racersNameOf2020;
	TArray<FString> nationalitiesOf2020;
	TArray<FString> racerTracksOf2020;

	//TODO public list of players, race tracks, nationalities, teams. To be replaced by players & team classes in fututre iteration
};
