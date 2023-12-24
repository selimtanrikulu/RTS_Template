// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SourcesWidget.generated.h"

class UStoreManager;
class UTileView;
/**
 * 
 */
UCLASS()
class RTSPLUGIN_API USourcesWidget : public UUserWidget
{
	GENERATED_BODY()

public:

	virtual void NativeConstruct() override;
	
	UPROPERTY(BlueprintReadOnly, meta=(BindWidget),Category="Main Widget")
	UTileView* SourceTileView;

	
private:

	//Dependencies
	UPROPERTY() UStoreManager* StoreManager;

	

	void CreateSourceEntries() const;
	void ClearWidget() const;
};
