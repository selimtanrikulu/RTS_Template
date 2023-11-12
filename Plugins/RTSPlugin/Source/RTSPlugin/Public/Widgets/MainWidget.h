// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Utility/Util.h"
#include "MainWidget.generated.h"

class UUnitGenerator;
struct FUnitData;
class UStoreManager;
class USelectionManager;
class UTileView;
/**
 * 
 */
UCLASS()
class RTSPLUGIN_API UMainWidget : public UUserWidget
{
	GENERATED_BODY()


public:

	virtual void NativeConstruct() override;
	
	UPROPERTY(BlueprintReadOnly, meta=(BindWidget),Category="Main Widget")
	UTileView* BuildingsTileView;

	UPROPERTY(BlueprintReadOnly, meta=(BindWidget),Category="Main Widget")
	UTileView* UnitsTileView;

private:

	//Dependencies
	UPROPERTY() UStoreManager* StoreManager;
	UPROPERTY() USelectionManager* SelectionManager;

	//Selection Manager Listener
	UFUNCTION()
	void OnSelectionChanged();


	void CreateUnitEntries(TArray<FUnitData> &UnitsData,UUnitGenerator* UnitGenerator) const;
	void CreateBuildingEntries(TArray<FBuildingData> &BuildingsData) const;
	
};
