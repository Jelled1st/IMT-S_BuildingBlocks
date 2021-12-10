// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ModularityComponent.h"
#include "RotationBehaviourComponent.generated.h"

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class BUILDINGBLOCKS_API URotationBehaviourComponent : public UModularityComponent
{
	GENERATED_BODY()

public:
	virtual void TickComponent(float deltaTime, ELevelTick tickType, FActorComponentTickFunction* thisTickFunction) override;

	UPROPERTY(EditAnywhere);
	double rotationSpeed = 10.0f;

protected:
	virtual void BeginPlay() override;

private:
	bool m_isRotating = false;
};
