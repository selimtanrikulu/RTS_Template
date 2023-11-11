// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MainWidget.generated.h"

class UBuildingManager;
class UTileView;
/**
 * 
 */
UCLASS()
class RTSPLUGIN_API UMainWidget : public UUserWidget
{
	GENERATED_BODY()


public:

	virtual void NativeConstruct() override;
	
	UPROPERTY(BlueprintReadOnly, meta=(BindWidget),Category="Store Widget")
	UTileView* BuildingsTileView;


private:

	//Dependencies
	UPROPERTY() UBuildingManager* BuildingManager;


	
};
