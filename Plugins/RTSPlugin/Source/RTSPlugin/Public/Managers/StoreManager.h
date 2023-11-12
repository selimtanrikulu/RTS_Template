// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "../Utility//Util.h"
#include "StoreManager.generated.h"

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
	           TSubclassOf<AActor> studioBP,
	           UWorld* world);
	
	void StartShopping();
	void StopShopping();
	void SelectNextCategory(const FString& Category);
	void SelectPreviousCategory();
	void SetDraftingBuilding(FBuildingData& BuildingData);
	bool EndOfTree() const;
	TArray<FString> GetCurrentChildren() const;
	TArray<FBuildingData> GetCurrentBuildings() const;

	TArray<FUnitData> GetCurrentUnits() const;

	TArray<FString> GetCurrentPath() const;

	void BuyDraftingObject() const;
	void CancelDraftingObject();


private:

	UPROPERTY() FStoreManagerConfig StoreManagerConfig;
	
	//Dependencies
	UPROPERTY() UWorld* World;
	UPROPERTY() UBuildingManager* BuildingManager;
	

	//Utility
	StoreTree* StoreTreeRoot;
	StoreTree* CurrentTree;
	FBuildingData* DraftingBuilding;


	void CreateImages();
	
	void CreateStoreTree();
	


	//Screenshot capture
	UPROPERTY() class UMaterial* BaseMaterial;


	UPROPERTY() TSubclassOf<AActor> StudioBP;
	

};
