// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/SourcesWidget.h"

#include "Components/TileView.h"
#include "Kismet/GameplayStatics.h"
#include "Managers/RTSGameInstance.h"
#include "Managers/StoreManager.h"
#include "Widgets/SourceEntry.h"

void USourcesWidget::NativeConstruct()
{
	Super::NativeConstruct();

	const APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(),0);
	const URTSGameInstance* GameInstance = Cast<URTSGameInstance>(PlayerController->GetGameInstance());

	StoreManager = GameInstance->StoreManager;


	ClearWidget();
	CreateSourceEntries();
}

void USourcesWidget::CreateSourceEntries() const
{
	TArray<FSourceData> SourcesData = StoreManager->GetSourcesData();

	for(const FSourceData SourceData : SourcesData)
	{
		//Create selection entry argument
		USourceEntryArgument* SourceEntryArgument = NewObject<USourceEntryArgument>();
		SourceEntryArgument->SourceData = SourceData;
		SourceEntryArgument->Amount = 10;
		SourceTileView->AddItem(SourceEntryArgument);
	}

	SourceTileView->SetVisibility(ESlateVisibility::Visible);
}

void USourcesWidget::ClearWidget() const
{
	SourceTileView->ClearListItems();
	SourceTileView->SetVisibility(ESlateVisibility::Hidden);
}
