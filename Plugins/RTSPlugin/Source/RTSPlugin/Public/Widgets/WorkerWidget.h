// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "WorkerWidget.generated.h"

struct FBuildingData;
class USelectionManager;
class UStoreManager;
class UTileView;
/**
 * 
 */
UCLASS()
class UWorkerWidget : public UUserWidget
{
	GENERATED_BODY()

public:

	virtual void NativeConstruct() override;
	
	UPROPERTY(BlueprintReadOnly, meta=(BindWidget),Category="Main Widget")
	UTileView* BuildingsTileView;


private:

	//Dependencies
	UPROPERTY() UStoreManager* StoreManager;
	UPROPERTY() USelectionManager* SelectionManager;

	//Selection Manager Listener
	UFUNCTION() void OnSelectionChanged();

	void CreateBuildingEntries(TArray<FBuildingData> &BuildingsData) const;
	void UpdateWidget() const;
	
	
};
