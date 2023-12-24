// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "Utility/Util.h"
#include "RTSGameInstance.generated.h"

class USourceManager;
class UEntityManager;
class UPlayerManager;
class UAsset_Manager;
class UStoreManager;
class ARTSHUD;
class UBuildingManager;
class USelectionManager;
class ULogManager;
/**
 * 
 */

class UManagerBase;

UCLASS()
class RTSPLUGIN_API URTSGameInstance : public UGameInstance
{
	GENERATED_BODY()

protected:

	virtual void Init() override;

public:

	//Managers
	UPROPERTY() ULogManager* LogManager;
	UPROPERTY() USelectionManager* SelectionManager;
	UPROPERTY() UBuildingManager* BuildingManager;
	UPROPERTY() ARTSHUD* RTSHUD;
	UPROPERTY() UStoreManager* StoreManager;
	UPROPERTY() UAsset_Manager* AssetManager;
	UPROPERTY() UPlayerManager* PlayerManager;
	UPROPERTY() UEntityManager* EntityManager;
	UPROPERTY() USourceManager* SourceManager;

	UPROPERTY() UWorld* World;

	
	//Store Manager Config
	//UPROPERTY(EditDefaultsOnly)
	//FStoreManagerConfig StoreManagerConfig;

	//Store Manager Config
	UPROPERTY(EditDefaultsOnly)
	TArray<FBuildingData> BuildingsData;

	UPROPERTY(EditDefaultsOnly)
	TArray<FUnitData> UnitsData;

	UPROPERTY(EditDefaultsOnly)
	TArray<FSourceData> SourcesData;

	UPROPERTY(EditDefaultsOnly)
	TArray<FSourceInfo> Sources;
	//--------------------------

	
	//Asset Manager Config
	UPROPERTY(EditDefaultsOnly)
	FAssetManagerConfig AssetManagerConfig;


	
private:
	
	//Level Calls
	UFUNCTION(BlueprintCallable)
	void OnBeginPlay();

	UFUNCTION(BlueprintCallable)
	void OnTick(float DeltaTime);


	//Managers
	TArray<UManagerBase*> Managers;

};
