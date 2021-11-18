// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "HAL/Runnable.h"
#include "Formula1Api.h"

class BUILDINGBLOCKS_API FFormula1ApiThreadHelper : public FRunnable
{
public:
	FFormula1ApiThreadHelper(UFormula1Api& f1Api);

	bool IsRunning();

	// FRunnable
	virtual bool Init() override;
	virtual uint32 Run() override;
	virtual void Stop() override;
	virtual void Exit() override;

private:
	bool m_isRunning = false;

	UPROPERTY();
	UFormula1Api* m_f1Api;
};
