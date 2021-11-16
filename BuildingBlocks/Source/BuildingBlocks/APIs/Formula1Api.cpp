// Fill out your copyright notice in the Description page of Project Settings.


#include "Formula1Api.h"

UFormula1Api::UFormula1Api()
{

}

void UFormula1Api::Init(FHttpModule& newHttpModule)
{
	m_httpModule = &newHttpModule;
}
