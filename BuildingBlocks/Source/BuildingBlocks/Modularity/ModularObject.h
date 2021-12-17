// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Containers/Map.h"
#include "ExposableParameterType.h"
#include "ModularObject.generated.h"

UCLASS()
class BUILDINGBLOCKS_API AModularObject : public AActor
{
	GENERATED_BODY()
	
public:	
	AModularObject();
	~AModularObject();

	void BeginDestroy() override;

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

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
	bool TrySetMeshByName(const FString& meshName);

	UMaterialInterface& GetMaterial();
	bool TrySetMaterialByName(const FString& materialName);

	void SetupParameter(bool& value, FString name);
	void SetupParameter(FString& value, FString name);
	void SetupParameter(double& value, FString name);
	TMap<FString, TPair<ExposableParameterType, void*>>& GetParameters();

	template <typename T>
	bool SetParameterValue(const FString& name, const T& value)
	{
		if (m_parameters.Contains(name))
		{
			TPair<ExposableParameterType, void*> parameter = m_parameters[name];

			T* parameterValue = reinterpret_cast<T*>(parameter.Value);

			*parameterValue = value;

			return true;
		}
		return false;
	}

	bool AllowsModularMeshes()
	{
		return m_allowModularMesh;
	}

private:
	int m_currentMeshIndex = 0;
	int m_currentMatIndex = 0;

	double posX, posY, posZ;
	double rotX, rotY, rotZ;
	double scaleX, scaleY, scaleZ;

	bool m_allowModularMesh = true;

	void SetupParameter(void* value, FString name, ExposableParameterType type);
	TMap<FString, TPair<ExposableParameterType, void*>> m_parameters;
};
