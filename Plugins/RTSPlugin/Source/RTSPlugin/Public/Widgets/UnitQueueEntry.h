// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Utility/Util.h"
#include "UnitQueueEntry.generated.h"

class UUnitGenerator;
class UButton;
class UImage;
class UTextBlock;


UCLASS(BlueprintType)
class RTSPLUGIN_API UUnitQueueEntryArgument : public UObject
{
	GENERATED_BODY()

public:
	//Building dependent
	FUnitData UnitData;

	UPROPERTY() UUnitGenerator* UnitGenerator;
	
	int Size;
	int QueueIndex;
};


/**
 * 
 */
UCLASS()
class RTSPLUGIN_API UUnitQueueEntry : public UUserWidget
{
	GENERATED_BODY()

public:

	virtual void NativeConstruct() override;
	
	UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
	UTextBlock* UnitNameText;

	UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
	UImage* UnitImage;

	UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
	UButton* EntryButton;

private:
	
	//Dependencies

	UUnitQueueEntryArgument* UnitQueueEntryArgument;
	
	UFUNCTION(BlueprintCallable)
	void StartEntry(UUnitQueueEntryArgument* unitQueueEntryArgument);

	UFUNCTION()
	void OnEntryButtonClicked();
	
};
