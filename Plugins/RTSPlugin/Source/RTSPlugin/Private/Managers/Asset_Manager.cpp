// Fill out your copyright notice in the Description page of Project Settings.


#include "Managers/Asset_Manager.h"

void UAsset_Manager::Begin(FAssetManagerConfig assetManagerConfig)
{
	AssetManagerConfig = assetManagerConfig;
}

TSubclassOf<AAIController> UAsset_Manager::GetAIControllerBP() const
{
	return AssetManagerConfig.AIControllerBP;
}
