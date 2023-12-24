// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "../Utility//Util.h"
#include "Managers/ManagerBase.h"
#include "StoreManager.generated.h"

class UEntityManager;
class UAsset_Manager;
class UBuildingManager;

class StoreTree;




UCLASS()
class RTSPLUGIN_API UStoreManager : public UManagerBase
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	UStoreManager();


	//Manager Base
	virtual void Init(URTSGameInstance* gameInstance) override;
	virtual void Begin() override;
	virtual void Tick(float DeltaTime) override;
	//--------------
	

	
	void OpenWorkerRoot();
	void CloseWorker();
	void SelectNextCategory(const FString& Category);
	void SelectPreviousCategory();
	void SetDraftingBuilding(const FBuildingData& BuildingData);
	bool EndOfTree() const;
	TArray<FString> GetCurrentChildren() const;
	TArray<FBuildingData> GetCurrentBuildings() const;
	TArray<FSourceInfo> GetSourcesInfo() const;

	TArray<FString> GetCurrentPath() const;
	

	FBuildingData* GetBuildingByName(const FString& BuildingName) const;
	FUnitData *GetUnitByName(const FString& UnitName) const;

private:

	
	//Dependencies
	UPROPERTY() UBuildingManager* BuildingManager;
	UPROPERTY() UAsset_Manager* AssetManager;
	UPROPERTY() UEntityManager* EntityManager;
	

	//Utility
	StoreTree* StoreTreeRoot;
	StoreTree* CurrentTree;

	void SetIDs() const;
	void FindExistingEntities() const;
	void CreateImagesPerspective();
	void CreateImagesOrthographic();
	void CreateStoreTree();

	TArray<FEntityData*> GetEntitiesData() const;


	

	//Screenshot capture
	UPROPERTY() class UMaterial* BaseMaterial;
	
	

};
