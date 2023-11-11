// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Utility/Util.h"
#include "BuildingEntry.generated.h"


class UBuildingManager;
class UButton;
class UTextBlock;

UCLASS(BlueprintType)
class RTSPLUGIN_API UBuildingEntryArgument : public UObject
{
	GENERATED_BODY()

public:
	//Building dependent
	FBuildingData BuildingData;
	
};


/**
 * 
 */
UCLASS()
class RTSPLUGIN_API UBuildingEntry : public UUserWidget
{
	GENERATED_BODY()



public:

	virtual void NativeConstruct() override;
	
	UPROPERTY(BlueprintReadOnly, meta=(BindWidget),Category="Store Widget")
	UTextBlock* BuildingNameText;

	UPROPERTY(BlueprintReadOnly, meta=(BindWidget),Category="Store Widget")
	UButton* EntryButton;

private:

	//Dependencies
	UPROPERTY() UBuildingManager* BuildingManager;


	UBuildingEntryArgument* BuildingEntryArgument;
	
	UFUNCTION(BlueprintCallable)
	void StartEntry(UBuildingEntryArgument* buildingEntryArgument);

	UFUNCTION()
	void OnEntryButtonClicked();

	
};
