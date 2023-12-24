// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Utility/Util.h"
#include "SourceEntry.generated.h"


class USourceManager;
class UImage;
class UTextBlock;

UCLASS(BlueprintType)
class RTSPLUGIN_API USourceEntryArgument : public UObject
{
	GENERATED_BODY()

public:

	//Entity dependent
	FSourceData SourceData;
	int Amount;
};

/**
 * 
 */
UCLASS()
class RTSPLUGIN_API USourceEntry : public UUserWidget
{
	GENERATED_BODY()

public:

	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;
	
	UPROPERTY(BlueprintReadOnly, meta=(BindWidget),Category="Store Widget")
	UTextBlock* SourceNameText;

	UPROPERTY(BlueprintReadOnly, meta=(BindWidget),Category="Store Widget")
	UTextBlock* SourceAmountText;

	UPROPERTY(BlueprintReadOnly, meta=(BindWidget),Category="Store Widget")
	UImage* SourceImage;


private:
	
	//Dependencies
	UPROPERTY() USourceManager* SourceManager;


	UPROPERTY() USourceEntryArgument* SourceEntryArgument;
	
	UFUNCTION(BlueprintCallable)
	void StartEntry(USourceEntryArgument* EntryArgument);
	

	void UpdateAmount() const;
	
	//Event Listeners
	UFUNCTION() void OnSourceUpdated();
	
};
