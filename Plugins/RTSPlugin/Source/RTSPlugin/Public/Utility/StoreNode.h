// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Util.h"

/**
 * 
 */
class RTSPLUGIN_API StoreNode
{
public:
	StoreNode(FString name);
	~StoreNode();

	//Only holds node data
	FString Name;

	TArray<FBuildingData> ContainingBuildings;

	void LogInfo() const;
};
