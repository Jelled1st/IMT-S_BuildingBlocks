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
		void HttpGetRequest();

	void OnResponseReceived(FHttpRequestPtr request, FHttpResponsePtr response, bool isSuccessful);

	//TODO public list of players, race tracks, nationalities, teams. To be replaced by players & team classes in fututre iteration
};
