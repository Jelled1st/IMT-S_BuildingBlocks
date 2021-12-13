// Fill out your copyright notice in the Description page of Project Settings.


#include "ModularUIActor.h"

// Sets default values
AModularUIActor::AModularUIActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AModularUIActor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AModularUIActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

UTextureRenderTarget2D* AModularUIActor::WidgetToTexture(UUserWidget* const widget, const FVector2D& drawSize)
{
    if (FSlateApplication::IsInitialized() && widget != nullptr)
    {
        // Get the slate widget as a smart pointer. Return if null.
        TSharedPtr<SWidget> SlateWidget(widget->TakeWidget());
        if (!SlateWidget) return nullptr;
        // Create a new widget renderer to render the widget to a texture render target 2D.
        FWidgetRenderer* WidgetRenderer = new FWidgetRenderer(true);
        if (!WidgetRenderer) return nullptr;
        // Update/Create the render target 2D.
        TextureRenderTarget = FWidgetRenderer::CreateTargetFor(drawSize, TF_Bilinear, false);


        WidgetRenderer->DrawWidget(TextureRenderTarget, SlateWidget.ToSharedRef(), drawSize, 0, false);
        // Return the updated render target 2D.
        return TextureRenderTarget;
    }
    else return nullptr;
}



