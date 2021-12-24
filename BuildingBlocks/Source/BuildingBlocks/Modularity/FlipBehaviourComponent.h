// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ModularityComponent.h"
#include "FlipBehaviourComponent.generated.h"

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class BUILDINGBLOCKS_API UFlipBehaviourComponent : public UModularityComponent
{
	GENERATED_BODY()

public:
	virtual void TickComponent(float deltaTime, ELevelTick tickType, FActorComponentTickFunction* thisTickFunction) override;

	UPROPERTY(EditAnywhere);
	double flipSpeed = 20.0f;

	UPROPERTY(EditAnywhere);
	double flipRotation = 180.0f;

protected:
	virtual void BeginPlay() override;

private:
	bool m_doFlip = false;
	bool m_inverseRotation = false;

	bool m_isFlipping = false;

	double currentRotation = 0;
};
