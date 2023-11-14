// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "../Utility//Util.h"
#include "StoreManager.generated.h"

class UAsset_Manager;
class UBuildingManager;

class StoreTree;




UCLASS()
class RTSPLUGIN_API UStoreManager : public UObject
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	UStoreManager();

	
	void Begin(const FStoreManagerConfig &storeManagerConfig,
	           UWorld* world);
	
	void OpenWorkerRoot();
	void CloseWorker();
	void SelectNextCategory(const FString& Category);
	void SelectPreviousCategory();
	void SetDraftingBuilding(FBuildingData& BuildingData);
	bool EndOfTree() const;
	TArray<FString> GetCurrentChildren() const;
	TArray<FBuildingData> GetCurrentBuildings() const;


	TArray<FString> GetCurrentPath() const;

	void BuyDraftingObject() const;
	void CancelDraftingObject();


	FBuildingData* GetBuildingByName(const FString& BuildingName);
	FUnitData *GetUnitByName(const FString& UnitName);

private:

	UPROPERTY() FStoreManagerConfig StoreManagerConfig;
	
	
	//Dependencies
	UPROPERTY() UWorld* World;
	UPROPERTY() UBuildingManager* BuildingManager;
	UPROPERTY() UAsset_Manager* AssetManager;
	

	//Utility
	StoreTree* StoreTreeRoot;
	StoreTree* CurrentTree;
	FBuildingData* DraftingBuilding;


	void SetIDs();
	void FindExistingEntities();
	void CreateImages();
	void CreateStoreTree();

	TArray<FEntityData*> GetEntitiesData();
	

	

	//Screenshot capture
	UPROPERTY() class UMaterial* BaseMaterial;
	
	

};
