// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Runtime/Online/HTTP/Public/Http.h"
#include "UObject/NoExportTypes.h"
#include "Formula1Api.generated.h"

UCLASS()
class BUILDINGBLOCKS_API UFormula1Api : public UObject
{
	GENERATED_BODY()
public:
	UFormula1Api();
	void Init(FHttpModule& newHttpModule);

private:
	UPROPERTY()
	FHttpModule* m_httpModule;
};
