// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Utility/Util.h"
#include "SelectionEntry.generated.h"


class UBuildingManager;
class UBuildingEntryArgument;
class UButton;
class UImage;
class UTextBlock;

UCLASS(BlueprintType)
class RTSPLUGIN_API USelectionEntryArgument : public UObject
{
	GENERATED_BODY()

public:

	//Entity dependent
	FEntityData EntityData;

	int Size;
};

/**
 * 
 */
UCLASS()
class RTSPLUGIN_API USelectionEntry : public UUserWidget
{
	GENERATED_BODY()

public:

	virtual void NativeConstruct() override;
	
	UPROPERTY(BlueprintReadOnly, meta=(BindWidget),Category="Store Widget")
	UTextBlock* SelectionNameText;

	UPROPERTY(BlueprintReadOnly, meta=(BindWidget),Category="Store Widget")
	UTextBlock* SelectionSizeText;

	UPROPERTY(BlueprintReadOnly, meta=(BindWidget),Category="Store Widget")
	UImage* SelectionImage;

	UPROPERTY(BlueprintReadOnly, meta=(BindWidget),Category="Store Widget")
	UButton* EntryButton;

private:

	//Dependencies
	UPROPERTY() UBuildingManager* BuildingManager;


	USelectionEntryArgument* SelectionEntryArgument;
	
	UFUNCTION(BlueprintCallable)
	void StartEntry(USelectionEntryArgument* selectionEntryArgument);

	UFUNCTION()
	void OnEntryButtonClicked();

	
};
