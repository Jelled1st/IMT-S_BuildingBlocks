// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Runtime/Online/HTTP/Public/Http.h"
#include "HttpTestActor.generated.h"

UCLASS()
class BUILDINGBLOCKS_API AHttpTestActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AHttpTestActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	FHttpModule* httpModule;

	
	UFUNCTION()
	void HttpGetRequest();

	void OnResponseReceived(FHttpRequestPtr request, FHttpResponsePtr response, bool isSuccessful);



};
