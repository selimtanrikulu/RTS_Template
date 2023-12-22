// Fill out your copyright notice in the Description page of Project Settings.


#include "Managers/Asset_Manager.h"

#include "Managers/RTSGameInstance.h"


void UAsset_Manager::Init(URTSGameInstance* gameInstance)
{
	Super::Init(gameInstance);

	AssetManagerConfig = GameInstance->AssetManagerConfig;
}

void UAsset_Manager::Begin()
{
	Super::Begin();
}

void UAsset_Manager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

TSubclassOf<AActor> UAsset_Manager::GetSelectBoxBP() const
{
	return AssetManagerConfig.SelectBoxBP;
}

TSubclassOf<AActor> UAsset_Manager::GetStudioBP() const
{
	return AssetManagerConfig.StudioBP;
}

TSubclassOf<AAIController> UAsset_Manager::GetAIControllerBP() const
{
	return AssetManagerConfig.AIControllerBP;
}

UMaterialInterface* UAsset_Manager::GetDraftMaterial() const
{
	return AssetManagerConfig.DraftMaterial;
}

UMaterialInterface* UAsset_Manager::GetErrorMaterial() const
{
	return AssetManagerConfig.ErrorMaterial;
}
