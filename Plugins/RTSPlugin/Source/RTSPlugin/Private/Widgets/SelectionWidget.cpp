// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/SelectionWidget.h"

#include "ActorsAndComponents/TeamEntity.h"
#include "Components/Image.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"
#include "Components/TileView.h"
#include "Kismet/GameplayStatics.h"
#include "Managers/RTSGameInstance.h"
#include "Managers/SelectionManager.h"
#include "Widgets/BuildingWidget.h"
#include "Widgets/SelectionEntry.h"

void USelectionWidget::NativeConstruct()
{
	Super::NativeConstruct();

	const APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(),0);
	const URTSGameInstance* GameInstance = Cast<URTSGameInstance>(PlayerController->GetGameInstance());
	
	SelectionManager = GameInstance->SelectionManager;
	StoreManager = GameInstance->StoreManager;
	
	SelectionManager->OnSelectionChangedDelegate.AddUniqueDynamic(this,&USelectionWidget::OnSelectionChanged);

	ClearWidget();
}

void USelectionWidget::OnSelectionChanged()
{
	ClearWidget();
	CreateSelectionEntries();
}

void USelectionWidget::CreateSelectionEntries() const
{
	TArray<URTSEntity*> SelectedEntities = SelectionManager->GetSelectedRTSEntities();
	
	if(SelectedEntities.Num() == 0)
	{
		return;
	}
	
	if(SelectedEntities.Num() == 1)
	{
		const URTSEntity* RTSEntity = SelectedEntities[0];
		const float Progress = RTSEntity->GetProgress();
		ProgressBar->SetPercent(Progress);
		ProgressBar->SetVisibility(ESlateVisibility::Visible);
	}

	const ESelectionState SelectionState = SelectionManager->GetSelectionState();
	if(SelectionState == ESelectionState::BuildingMass ||
		SelectionState == ESelectionState::BuildingSingle ||
		SelectionState == ESelectionState::UnitMass ||
		SelectionState == ESelectionState::UnitSingle ||
		SelectionState == ESelectionState::SourceSingle ||
		SelectionState == ESelectionState::SourceMass)
	{
		const URTSEntity* RTSEntity = SelectedEntities[0];
		const FString Info = RTSEntity->GetInfo();
		InfoText->SetText(FText::FromString(Info));
		InfoText->SetVisibility(ESlateVisibility::Visible);
	}
	
	while(SelectedEntities.Num() > 0)
	{
		const URTSEntity* RTSEntity = SelectedEntities[0];

		//Get all entities with id = Entity.ID
		const int Size = SelectedEntities.FilterByPredicate([&](const URTSEntity* Element)
			{ return Element->GetEntityData().EntityID == RTSEntity->GetEntityData().EntityID;}).Num();

		//Remove all such elements
		SelectedEntities.RemoveAll([&](const URTSEntity* Element)
			{ return Element->GetEntityData().EntityID == RTSEntity->GetEntityData().EntityID; });

		//Create selection entry argument
		USelectionEntryArgument* SelectionEntryArgument =
			NewObject<USelectionEntryArgument>();
		SelectionEntryArgument->EntityData = RTSEntity->GetEntityData();
		SelectionEntryArgument->Size = Size;
		SelectionTileView->AddItem(SelectionEntryArgument);
	}
	
	SelectionTileView->SetVisibility(ESlateVisibility::Visible);
	PanelBackground->SetVisibility(ESlateVisibility::Visible);
}

void USelectionWidget::ClearWidget() const
{
	InfoText->SetText(FText::FromString(""));
	ProgressBar->SetPercent(0);
	SelectionTileView->ClearListItems();

	InfoText->SetVisibility(ESlateVisibility::Hidden);
	ProgressBar->SetVisibility(ESlateVisibility::Hidden);
	SelectionTileView->SetVisibility(ESlateVisibility::Hidden);
	PanelBackground->SetVisibility(ESlateVisibility::Hidden);
}
