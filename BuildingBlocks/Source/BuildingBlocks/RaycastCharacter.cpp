// Fill out your copyright notice in the Description page of Project Settings.


#include "RaycastCharacter.h"
#include "Debug/Debug.h"
#include "ModularObject.h"

// Sets default values
ARaycastCharacter::ARaycastCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ARaycastCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

void ARaycastCharacter::RayCast()
{
	FHitResult* hit = new FHitResult();
	FVector forward = GetWorld()->GetFirstPlayerController()->PlayerCameraManager->GetActorForwardVector();
	FVector start = GetActorLocation();
	start.Z = 0;
	FVector end = start + (forward * 1000.f);

	if (GetWorld()->LineTraceSingleByChannel(*hit, start, end, ECC_WorldStatic)) 
	{
		DrawDebugLine(GetWorld(), start, end, FColor::Red, true);

		if (hit->GetActor() != NULL) 
		{
			Debug::Log(FString::Printf(TEXT("%s: %s"), *hit->GetActor()->GetName(), *hit->GetComponent()->GetName()));

			AModularObject* modularObj = Cast<AModularObject>(hit->GetActor());
			if (modularObj != nullptr)
			{
				modularObj->Select();
			}
		}
	}
}

// Called every frame
void ARaycastCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ARaycastCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	InputComponent->BindAction("Raycast_Button", IE_Pressed, this, &ARaycastCharacter::RayCast);

}

