// Fill out your copyright notice in the Description page of Project Settings.


#include "RaycastActor.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Character.h"


// Sets default values
ARaycastActor::ARaycastActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	


}

// Called when the game starts or when spawned
void ARaycastActor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ARaycastActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


void ARaycastActor::RayCast() {
	FHitResult* hit = new FHitResult();
	FVector forward = GetWorld()->GetFirstPlayerController()->PlayerCameraManager->GetActorForwardVector();
	FVector start = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0)->GetActorLocation();
	start.Z = 100.f;
	FVector end = (forward * 1000.f) + start;

	if (GetWorld()->LineTraceSingleByChannel(*hit, start, end, ECC_WorldStatic)) {
		DrawDebugLine(GetWorld(), start, end, FColor::Red, true);

		if (hit->GetActor() != NULL) {
			UE_LOG(LogClass, Log, TEXT("Component: %s"), *hit->GetComponent()->GetName());
		}
	}


}

