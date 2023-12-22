// Fill out your copyright notice in the Description page of Project Settings.

#pragma once



#include "CoreMinimal.h"
#include "Managers/ManagerBase.h"
#include "SelectionManager.generated.h"

class UPlayerManager;
class UAsset_Manager;
class AUnit;
class ABuilding;
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnSelectionChanged);


class ULogManager;
class ASelectBox;
class ARTSPawn;
/**
 * 
 */


UENUM(BlueprintType)
enum class ESelectionState : uint8
{
	None,
	UnitSingle,
	UnitMass,
	UnitMixed,
	BuildingSingle,
	BuildingMass,
	BuildingMixed,
	AllMixed,
};





UCLASS()
class RTSPLUGIN_API USelectionManager : public UManagerBase
{
	GENERATED_BODY()

public:
	USelectionManager();
	~USelectionManager();

	//Manager Base
	virtual void Init(URTSGameInstance* gameInstance) override;
	virtual void Begin() override;
	virtual void Tick(float DeltaTime) override;
	//--------------


	TArray<ABuilding*> GetSelectedBuildings() const;
	TArray<AUnit*> GetSelectedUnits() const;


	ESelectionState GetSelectionState() const;
	

	UPROPERTY(BlueprintAssignable) FOnSelectionChanged OnSelectionChangedDelegate;

private:

	//Dependencies
	UPROPERTY() UPlayerManager* PlayerManager;
	UPROPERTY() ULogManager* LogManager;
	UPROPERTY() UAsset_Manager* AssetManager;
	

	//Input listeners
	UFUNCTION() void OnMouseLeftClicked();
	UFUNCTION() void OnMouseLeftReleased();
	UFUNCTION() void OnMouseRightClicked();
	UFUNCTION() void OnDeleteClicked();

	//Select Box Listeners
	UFUNCTION() void OnOverlapChanged();
	

	void UpdateCircles();
	void UpdateSelectionState();
	

	//Utility
	UPROPERTY() ASelectBox* CurrentSelectBox;

	ESelectionState SelectionState;


	TArray<ABuilding*> SelectedBuildings;
	TArray<AUnit*> SelectedUnits;
	
	
	void CreateSelectBox();
	void DestroySelectBox();

	//Utility functions
	bool AreSameBuildings(TArray<ABuilding*> Buildings);
	bool AreSameUnits(TArray<AUnit*> Units);
	
	
};
