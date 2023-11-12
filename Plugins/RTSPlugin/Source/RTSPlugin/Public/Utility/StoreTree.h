// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "StoreNode.h"

/**
 * 
 */
class RTSPLUGIN_API StoreTree
{
public:
	StoreTree(StoreNode* node, StoreTree* parent);
	~StoreTree();

	StoreNode* Node;
	StoreTree* Parent;
	TArray<StoreTree*> Children;
	

	void AddCategory(TArray<FString> &Path,  FBuildingData& BuildingData);


	void LogInfo();


	TArray<TArray<FString>> GetAllPaths();
	TArray<FBuildingData> GetBuildingsByPath(TArray<FString>& Path);
	TArray<FString> GetChildrenNames();
	StoreTree* GetStoreTreeByPath(TArray<FString> &Path);
	StoreTree* GetChildWithName(const FString& Category);

private:

	


	static TArray<FString> GetTail(TArray<FString> &Path);
	
};
