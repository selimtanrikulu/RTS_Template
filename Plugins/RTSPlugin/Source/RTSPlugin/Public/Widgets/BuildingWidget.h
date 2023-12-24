// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Utility/Util.h"
#include "BuildingWidget.generated.h"

class UImage;
class UUnitGenerator;
class ABuilding;
class USelectionManager;
class UStoreManager;
class UProgressBar;
class UTileView;
/**
 * 
 */
UCLASS()
class UBuildingWidget : public UUserWidget
{
	GENERATED_BODY()

public:

	virtual void NativeConstruct() override;

	UPROPERTY(BlueprintReadOnly, meta=(BindWidget),Category="Building Widget")
	UImage* PanelBackground;
	
	UPROPERTY(BlueprintReadOnly, meta=(BindWidget),Category="Building Widget")
	UTileView* UnitsTileView;

private:

	//Dependencies
	UPROPERTY() UStoreManager* StoreManager;
	UPROPERTY() USelectionManager* SelectionManager;

	//Selection Manager Listener
	UFUNCTION() void OnSelectionChanged();

	void CreateUnitEntries(TArray<FUnitData> &UnitsData,UUnitGenerator* UnitGenerator) const;
	
	void ClearWidget() const;
	
};
