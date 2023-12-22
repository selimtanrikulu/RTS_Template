// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Managers/ManagerBase.h"
#include "Utility/Util.h"
#include "Asset_Manager.generated.h"

/**
 * 
 */
UCLASS()
class RTSPLUGIN_API UAsset_Manager : public UManagerBase
{
	GENERATED_BODY()

	

public:

	//Manager Base
	virtual void Init(URTSGameInstance* gameInstance) override;
	virtual void Begin() override;
	virtual void Tick(float DeltaTime) override;
	//--------------

	
	TSubclassOf<AActor> GetSelectBoxBP() const;
	TSubclassOf<AActor> GetStudioBP() const;
	TSubclassOf<AAIController> GetAIControllerBP() const;

	UMaterialInterface* GetDraftMaterial() const;
	UMaterialInterface* GetErrorMaterial() const;
	
	FAssetManagerConfig AssetManagerConfig;


	
};
