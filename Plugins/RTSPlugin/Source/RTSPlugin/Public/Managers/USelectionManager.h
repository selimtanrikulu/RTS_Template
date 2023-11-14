// Fill out your copyright notice in the Description page of Project Settings.

#pragma once



#include "CoreMinimal.h"
#include "USelectionManager.generated.h"

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
class RTSPLUGIN_API USelectionManager : public UObject
{
	GENERATED_BODY()

public:
	USelectionManager();
	~USelectionManager();

	void Begin(UWorld* world);
	void Tick(float DeltaTime);


	TArray<ABuilding*> GetSelectedBuildings() const;
	TArray<AUnit*> GetSelectedUnits() const;


	ESelectionState GetSelectionState() const;
	

	UPROPERTY(BlueprintAssignable) FOnSelectionChanged OnSelectionChangedDelegate;

private:

	//Dependencies
	UPROPERTY() UWorld* World;
	UPROPERTY() ARTSPawn* RTSPawn;
	UPROPERTY() ULogManager* LogManager;
	UPROPERTY() UAsset_Manager* AssetManager;
	

	//Input listeners
	UFUNCTION()
	void OnMouseLeftClicked();
	UFUNCTION()
	void OnMouseLeftReleased();
	UFUNCTION()
	void OnMouseRightClicked();

	//Select Box Listeners
	UFUNCTION()
	void OnOverlapChanged();


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
