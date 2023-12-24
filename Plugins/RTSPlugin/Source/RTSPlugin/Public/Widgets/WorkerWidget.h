// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "WorkerWidget.generated.h"

class UImage;
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

	UPROPERTY(BlueprintReadOnly, meta=(BindWidget),Category="Worker Widget")
	UImage* PanelBackground;
	
	UPROPERTY(BlueprintReadOnly, meta=(BindWidget),Category="Worker Widget")
	UTileView* BuildingsTileView;


private:

	//Dependencies
	UPROPERTY() UStoreManager* StoreManager;
	UPROPERTY() USelectionManager* SelectionManager;

	//Selection Manager Listener
	UFUNCTION() void OnSelectionChanged();

	void CreateBuildingEntries() const;
	void UpdateWidget() const;

	void ClearWidget() const;
	
};
