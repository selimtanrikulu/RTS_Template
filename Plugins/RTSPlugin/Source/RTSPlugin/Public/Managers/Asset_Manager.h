// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Utility/Util.h"
#include "Asset_Manager.generated.h"

/**
 * 
 */
UCLASS()
class RTSPLUGIN_API UAsset_Manager : public UObject
{
	GENERATED_BODY()

	

public:

	void Begin(FAssetManagerConfig assetManagerConfig);

	TSubclassOf<AActor> GetSelectBoxBP() const;
	TSubclassOf<AActor> GetStudioBP() const;
	TSubclassOf<AAIController> GetAIControllerBP() const;

	UMaterialInterface* GetDraftMaterial() const;
	UMaterialInterface* GetErrorMaterial() const;
	
	FAssetManagerConfig AssetManagerConfig;


	
};
