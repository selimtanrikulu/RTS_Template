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
	virtual void NativeDestruct() override;

	UPROPERTY(BlueprintReadOnly, meta=(BindWidget),Category="Building Widget")
	UImage* PanelBackground;
	
	UPROPERTY(BlueprintReadOnly, meta=(BindWidget),Category="Building Widget")
	UTileView* UnitsTileView;


	UPROPERTY(BlueprintReadOnly, meta=(BindWidget),Category="Building Widget")
	UProgressBar* GenerationProgressBar;

	UPROPERTY(BlueprintReadOnly, meta=(BindWidget),Category="Building Widget")
	UTileView* UnitQueueTileView;
	
	
private:

	//Dependencies
	UPROPERTY() UStoreManager* StoreManager;
	UPROPERTY() USelectionManager* SelectionManager;

	//Selection Manager Listener
	UFUNCTION() void OnSelectedEntitiesChanged();
	UFUNCTION() void OnBuildingStateChanged(ABuilding* Building);
	UFUNCTION() void OnUnitGenerationProgressUpdated();
	UFUNCTION() void OnUnitQueueUpdated();

	void CreateUnitEntries(TArray<FUnitData> &UnitsData,UUnitGenerator* UnitGenerator) const;
	void CreateQueueEntries(TArray<FUnitData> &UnitQueue,UUnitGenerator* UnitGenerator) const;


	void HardReset();
	
};
