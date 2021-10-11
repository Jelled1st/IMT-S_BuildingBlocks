// Fill out your copyright notice in the Description page of Project Settings.


#include "LevevlBehaviour.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h" 
#include "Kismet/GameplayStatics.h"
#include "Components/BoxComponent.h"
#include "GameFramework/Character.h"

// Sets default values
ALevevlBehaviour::ALevevlBehaviour()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	OverlapVolume = CreateDefaultSubobject<UBoxComponent>(TEXT("OverlapVolume"));
	RootComponent = OverlapVolume;
	OverlapVolume->OnComponentBeginOverlap.AddUniqueDynamic(this, &ALevevlBehaviour::OverlapBegins);
	OverlapVolume->OnComponentEndOverlap.AddUniqueDynamic(this, &ALevevlBehaviour::OverlapEnds);

}

void ALevevlBehaviour::LoadLevel()
{

	UGameplayStatics::OpenLevel(GetWorld(), "Scene_Blockout");
}

// Called when the game starts or when spawned
void ALevevlBehaviour::BeginPlay()
{
	Super::BeginPlay();
	//LoadLevel();
	
}

// Called every frame
void ALevevlBehaviour::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ALevevlBehaviour::OverlapBegins(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if(UGameplayStatics::GetPlayerCharacter(this,0)!=NULL)
	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("Some debug message!"));

	ACharacter* MyCharacter = UGameplayStatics::GetPlayerCharacter(this, 0);
	//if (OtherActor == MyCharacter && LevelToLoad != "")
	//{
	//	FLatentActionInfo LatentInfo;
	//	UGameplayStatics::LoadStreamLevel(this, LevelToLoad, true, true, LatentInfo);
	//}

	if ( LevelToLoad != "")
	{
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("Some debug message!"));
		FLatentActionInfo LatentInfo;
		UGameplayStatics::LoadStreamLevel(this, LevelToLoad, true, true, LatentInfo);
	}
}

void ALevevlBehaviour::OverlapEnds(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	ACharacter* MyCharacter = UGameplayStatics::GetPlayerCharacter(this, 0);
	/*if (OtherActor == MyCharacter && LevelToLoad != "")
	{
		FLatentActionInfo LatentInfo;
		UGameplayStatics::UnloadStreamLevel(this, LevelToLoad, LatentInfo);
	}*/


	if (LevelToLoad != "")
	{
		FLatentActionInfo LatentInfo;
		UGameplayStatics::UnloadStreamLevel(this, LevelToLoad, LatentInfo,true);
	}
}

