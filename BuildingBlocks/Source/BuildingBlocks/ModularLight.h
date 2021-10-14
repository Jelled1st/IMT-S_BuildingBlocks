// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ModularObject.h"
#include "ModularLight.generated.h"

UCLASS()
class BUILDINGBLOCKS_API AModularLight : public AModularObject
{
	GENERATED_BODY()
public:
	AModularLight();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float deltaTime) override;

	UPROPERTY(EditAnywhere);
	bool lightIsEnabled = false;

private:
	bool m_isLightEnabled = false;
};
