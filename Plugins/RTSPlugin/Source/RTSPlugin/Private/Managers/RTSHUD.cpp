// Fill out your copyright notice in the Description page of Project Settings.


#include "Managers/RTSHUD.h"
#include "Blueprint/UserWidget.h"
#include "Managers/RTSGameInstance.h"
#include "UObject/ConstructorHelpers.h"

ARTSHUD::ARTSHUD()
{
	static ConstructorHelpers::FClassFinder<UUserWidget> MainWidgetAsset(TEXT("/RTSPlugin/Blueprints/Widget/WBP_Main") );
	if(MainWidgetAsset.Succeeded())
	{
		MainWidgetBP = MainWidgetAsset.Class;
	}
}

void ARTSHUD::BeginPlay()
{
	Super::BeginPlay();

	GameInstance = Cast<URTSGameInstance>(GetGameInstance());
}


void ARTSHUD::SetWidget(EWidgetType WidgetType)
{
	TSubclassOf<UUserWidget> SelectedWidget;
	
	switch (WidgetType)
	{
		case EWidgetType::None:
			SelectedWidget = nullptr;
			break;

		case EWidgetType::Main:
			SelectedWidget = MainWidgetBP;
			break;

		default:
			UE_LOG(LogTemp,Warning,TEXT("Widget not set for widget type"));
			return;
	}

	if(!SelectedWidget)
	{
		UE_LOG(LogTemp,Warning,TEXT("Null widget selected"));
		return;
	}



	//Remove previous one, if exists
	if(CurrentWidget)
	{
		UE_LOG(LogTemp,Display,TEXT("Removing previous widget"));
		CurrentWidget->RemoveFromViewport();
		CurrentWidget->Destruct();
	}
	
	UUserWidget* NewWidget = CreateWidget<UUserWidget>( GetWorld(),SelectedWidget);
	if(NewWidget)
	{
		NewWidget->AddToViewport();
	}
	

	CurrentWidget = NewWidget;
}
