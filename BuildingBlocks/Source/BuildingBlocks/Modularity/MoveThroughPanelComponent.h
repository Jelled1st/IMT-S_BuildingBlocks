// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ModularityComponent.h"
#include "FlipBehaviourComponent.h"
#include "MoveThroughPanelComponent.generated.h"

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class BUILDINGBLOCKS_API UMoveThroughPanelComponent : public UModularityComponent
{
	GENERATED_BODY()
	
public:
	virtual void BeginPlay() override;

	virtual void TickComponent(float deltaTime, ELevelTick tickType, FActorComponentTickFunction* thisTickFunction);

	void OnPanelFlip(UFlipBehaviourComponent* flipBehaviour);

	UPROPERTY(EditAnywhere);
	TSoftObjectPtr<AActor> panelSoftPtr;

	UPROPERTY(EditAnywhere);
	float size;

private:
	bool m_isMoving = false;

	float m_speed;
	float m_currentMovement;

	UPROPERTY(); 
	UFlipBehaviourComponent* m_flipBehaviour;
};
