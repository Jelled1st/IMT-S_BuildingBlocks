// Fill out your copyright notice in the Description page of Project Settings.

#include "HttpTestActor.h"
#include "Runtime/Engine/Public/EngineGlobals.h"
#include "Runtime/Online/HTTP/Public/Http.h"

// Sets default values
AHttpTestActor::AHttpTestActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	httpModule = &FHttpModule::Get();

}

// Called when the game starts or when spawned
void AHttpTestActor::BeginPlay()
{
	Super::BeginPlay();
	HttpGetRequest();
	
}

// Called every frame
void AHttpTestActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AHttpTestActor::HttpGetRequest()
{
	TSharedRef<IHttpRequest, ESPMode::ThreadSafe> Request = httpModule->CreateRequest();
	Request->OnProcessRequestComplete().BindUObject(this, &AHttpTestActor::OnResponseReceived);

	//process GET request
	Request->SetURL("https://ergast.com/api/f1/1988.json");
	Request->SetVerb("GET");
	Request->SetHeader(TEXT("User-Agent"), "X-UnrealEngine-Agent");
	Request->SetHeader("Content-Type", TEXT("application/json"));
	Request->ProcessRequest();
}

void AHttpTestActor::OnResponseReceived(FHttpRequestPtr request, FHttpResponsePtr response, bool isSuccessful)
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

