// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Utility/Util.h"
#include "BuildingManager.generated.h"

class ABuilding;
class ARTSPawn;
/**
 * 
 */
UCLASS()
class RTSPLUGIN_API UBuildingManager : public UObject
{
	GENERATED_BODY()


public:

	void Begin(UWorld* world);
	void Tick(float DeltaTime);


	void DraftBuilding(const FBuildingData& BuildingData);

	bool IsDrafting() const;

private:
	//Dependencies
	UPROPERTY() UWorld* World;
	UPROPERTY() ARTSPawn* RTSPawn;


	//Input listeners
	UFUNCTION() void OnMouseLeftClicked();
	UFUNCTION() void OnMouseRightClicked();
	UFUNCTION() void OnMouseWheelUp();
	UFUNCTION() void OnMouseWheelDown();
	


	UPROPERTY() ABuilding* DraftingBuilding;


	void UpdateDraftingBuildingLocation() const;
	void LocateBuilding();
	void CancelDraft();
	void RotateBuilding(float Amount) const;
	
	
};
