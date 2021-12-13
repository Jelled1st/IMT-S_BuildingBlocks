// Fill out your copyright notice in the Description page of Project Settings.

#pragma once



#include "CoreMinimal.h"

#include "Engine/TextureRenderTarget2D.h"
#include "Blueprint/UserWidget.h"
#include "Slate/WidgetRenderer.h"
#include "Math/Vector2D.h"

#include "GameFramework/Actor.h"
#include "ModularUIActor.generated.h"


UCLASS(Blueprintable)
class BUILDINGBLOCKS_API AModularUIActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AModularUIActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	UFUNCTION(BlueprintCallable, Category = RenderTexture)
	 UTextureRenderTarget2D* WidgetToTexture( UUserWidget* const widget, const FVector2D& drawSize);

	///* UPROPERTY initialized so UE4 will handle its garbage collection correctly... */
	UPROPERTY(BlueprintReadWrite, Category = RenderTexture)
	 UTextureRenderTarget2D* TextureRenderTarget;
};
