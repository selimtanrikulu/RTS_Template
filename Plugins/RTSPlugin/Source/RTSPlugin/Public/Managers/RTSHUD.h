// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "Blueprint/UserWidget.h"
#include "RTSHUD.generated.h"


UENUM(BlueprintType)
enum class EWidgetType : uint8
{
	None,
	Main,
};



/**
 * 
 */
UCLASS()
class RTSPLUGIN_API ARTSHUD : public AHUD
{
	GENERATED_BODY()

public:
	
	ARTSHUD();

	virtual void BeginPlay() override;

	void SetWidget(EWidgetType WidgetType);
	
private:
	
	TSubclassOf<UUserWidget> MainWidgetBP;


	UUserWidget* CurrentWidget;
	
};
