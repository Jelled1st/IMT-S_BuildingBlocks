// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ModularObject.generated.h"

UCLASS()
class BUILDINGBLOCKS_API AModularObject : public AActor
{
	GENERATED_BODY()
	
public:	
	AModularObject();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;
	virtual void BeginDestroy() override;

	UPROPERTY(EditAnywhere);
	UStaticMeshComponent* visualComponent;

	UPROPERTY(EditAnywhere);
	TArray<UStaticMesh*> meshAssets;

	UPROPERTY(EditAnywhere);
	TArray<UMaterialInterface*> materialAssets;

	UFUNCTION(BlueprintCallable)
	void SwapMeshNext();

	UFUNCTION(BlueprintCallable)
	void SwapMeshPrevious();
	bool SwapMesh(int index);

	UFUNCTION(BlueprintCallable)
	void SwapMatNext();

	UFUNCTION(BlueprintCallable)
	void SwapMatPrevious();
	bool SwapMat(int index);

	UStaticMesh& GetMesh();
	UMaterialInterface& GetMaterial();

private:
	int m_currentMeshIndex = 0;
	int m_currentMatIndex = 0;
};
