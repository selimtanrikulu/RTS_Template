// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Utility/Util.h"
#include "UnitEntry.generated.h"


class UButton;
class UTextBlock;
class UImage;

UCLASS(BlueprintType)
class RTSPLUGIN_API UUnitEntryArgument : public UObject
{
	GENERATED_BODY()

public:
	//Building dependent
	FUnitData UnitData;
	
};


/**
 * 
 */
UCLASS()
class RTSPLUGIN_API UUnitEntry : public UUserWidget
{
	GENERATED_BODY()


public:

	virtual void NativeConstruct() override;
	
	UPROPERTY(BlueprintReadOnly, meta=(BindWidget),Category="Store Widget")
	UTextBlock* UnitNameText;

	UPROPERTY(BlueprintReadOnly, meta=(BindWidget),Category="Store Widget")
	UImage* UnitImage;

	UPROPERTY(BlueprintReadOnly, meta=(BindWidget),Category="Store Widget")
	UButton* EntryButton;

private:
	
	//Dependencies

	UUnitEntryArgument* UnitEntryArgument;
	
	UFUNCTION(BlueprintCallable)
	void StartEntry(UUnitEntryArgument* unitEntryArgument);

	UFUNCTION()
	void OnEntryButtonClicked();
	
};
