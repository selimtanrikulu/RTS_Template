// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Managers/ManagerBase.h"
#include "Utility/Util.h"
#include "BuildingManager.generated.h"

class UEntityManager;
class UPlayerManager;
class ABuilding;
class ARTSPawn;
/**
 * 
 */
UCLASS()
class RTSPLUGIN_API UBuildingManager : public UManagerBase
{
	GENERATED_BODY()


public:

	//Manager Base
	virtual void Init(URTSGameInstance* gameInstance) override;
	virtual void Begin() override;
	virtual void Tick(float DeltaTime) override;
	//--------------




	void DraftBuilding(const FBuildingData& BuildingData);

	bool IsDrafting() const;

private:
	//Dependencies
	UPROPERTY() UPlayerManager* PlayerManager;
	UPROPERTY() UEntityManager* EntityManager;


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
