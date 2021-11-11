// Fill out your copyright notice in the Description page of Project Settings.


#include "HTTPTestObject.h"
#include "Runtime/Engine/Public/EngineGlobals.h"
#include "Runtime/Online/HTTP/Public/Http.h"


UHTTPTestObject::UHTTPTestObject() {

	httpModule = &FHttpModule::Get();
}
void UHTTPTestObject::Start()
{
	//TODO: public callable htttp get request
	HttpGetRequest();

	
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
