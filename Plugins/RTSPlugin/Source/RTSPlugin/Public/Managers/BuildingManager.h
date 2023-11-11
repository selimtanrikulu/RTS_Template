// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Utility/Util.h"
#include "BuildingManager.generated.h"

class ARTSPawn;
/**
 * 
 */
UCLASS()
class RTSPLUGIN_API UBuildingManager : public UObject
{
	GENERATED_BODY()


public:

	void Begin(UWorld* world,const FBuildingManagerConfig& buildingManagerConfig);
	void Tick(float DeltaTime);

	TArray<FBuildingData> GetBuildingsData() const;



	void DraftBuilding(const FBuildingData& BuildingData);

private:
	//Dependencies
	UPROPERTY() UWorld* World;
	UPROPERTY() ARTSPawn* RTSPawn;


	//Input listeners
	UFUNCTION() void OnMouseLeftClicked();
	UFUNCTION() void OnMouseRightClicked();
	UFUNCTION() void OnMouseWheelUp();
	UFUNCTION() void OnMouseWheelDown();
	

	
	FBuildingManagerConfig BuildingManagerConfig;

	UPROPERTY() AActor* DraftingBuilding;


	void UpdateDraftingBuildingLocation() const;
	void LocateBuilding();
	void CancelDraft();
	void RotateBuilding(float Amount) const;
	
	
};
