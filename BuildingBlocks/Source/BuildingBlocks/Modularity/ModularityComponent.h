// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GameFramework/Actor.h"
#include "Containers/Map.h"
#include "ExposableParameterType.h"
#include "ModularityComponent.generated.h"

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BUILDINGBLOCKS_API UModularityComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UModularityComponent();
	~UModularityComponent();

	void BeginDestroy() override;

protected:
	virtual void BeginPlay() override;

public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UPROPERTY(EditAnywhere);
	UStaticMeshComponent* visualComponent;

	UPROPERTY(EditAnywhere);
	TArray<UStaticMesh*> meshAssets;

	UPROPERTY(EditAnywhere);
	TArray<UMaterialInterface*> materialAssets;

	FString GetActorName() const;

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

	bool RemoveParameter(const FString& name);

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

	void SetWorldPosition(FVector pos)
	{
		posX = pos.X;
		posY = pos.Y;
		posZ = pos.Z;
	}

	bool AllowsModularMeshes()
	{
		return m_allowModularMesh;
	}

protected:
	UPROPERTY();
	AActor* m_owner;

	int m_currentMeshIndex = 0;
	int m_currentMatIndex = 0;

	double posX, posY, posZ;
	double rotX, rotY, rotZ;
	double scaleX, scaleY, scaleZ;

	bool m_allowModularMesh = true;

	void SetupParameter(void* value, FString name, ExposableParameterType type);
	TMap<FString, TPair<ExposableParameterType, void*>> m_parameters;
};
