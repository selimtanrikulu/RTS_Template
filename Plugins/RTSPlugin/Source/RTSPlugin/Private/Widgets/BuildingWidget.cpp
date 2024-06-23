// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/BuildingWidget.h"

#include "ActorsAndComponents/Building.h"
#include "ActorsAndComponents/UnitGenerator.h"
#include "Components/Image.h"
#include "Components/ProgressBar.h"
#include "Components/TileView.h"
#include "Kismet/GameplayStatics.h"
#include "Managers/RTSGameInstance.h"
#include "Managers/SelectionManager.h"
#include "Widgets/BuildingEntry.h"
#include "Widgets/UnitEntry.h"
#include "Widgets/UnitQueueEntry.h"


void UBuildingWidget::NativeConstruct()
{
	Super::NativeConstruct();

	const APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(),0);
	const URTSGameInstance* GameInstance = Cast<URTSGameInstance>(PlayerController->GetGameInstance());

	
	SelectionManager = GameInstance->SelectionManager;
	StoreManager = GameInstance->StoreManager;
	
	SelectionManager->OnSelectedEntitiesChangedDelegate.AddUniqueDynamic(this,&UBuildingWidget::OnSelectedEntitiesChanged);
	SelectionManager->OnBuildingStateChangedDelegate.AddUniqueDynamic(this,&UBuildingWidget::OnBuildingStateChanged);
	SelectionManager->OnUnitGenerationProgressUpdatedDelegate.AddUniqueDynamic(this,&UBuildingWidget::OnUnitGenerationProgressUpdated);
	SelectionManager->OnUnitQueueUpdatedDelegate.AddUniqueDynamic(this,&UBuildingWidget::OnUnitQueueUpdated);


	//Clear widget
	GenerationProgressBar->SetVisibility(ESlateVisibility::Hidden);
	UnitQueueTileView->ClearListItems();
	UnitQueueTileView->SetVisibility(ESlateVisibility::Hidden);
	
	UnitsTileView->ClearListItems();
	UnitsTileView->SetVisibility(ESlateVisibility::Hidden);
	PanelBackground->SetVisibility(ESlateVisibility::Hidden);
}

void UBuildingWidget::NativeDestruct()
{
	Super::NativeDestruct();

	SelectionManager->OnSelectedEntitiesChangedDelegate.RemoveDynamic
	(this,&UBuildingWidget::OnSelectedEntitiesChanged);
	SelectionManager->OnUnitGenerationProgressUpdatedDelegate.RemoveDynamic
	(this,&UBuildingWidget::OnUnitGenerationProgressUpdated);
}


void UBuildingWidget::OnSelectedEntitiesChanged()
{
	HardReset();	
}

void UBuildingWidget::OnBuildingStateChanged(ABuilding* Building)
{
	HardReset();	
}

void UBuildingWidget::OnUnitGenerationProgressUpdated()
{
	GenerationProgressBar->SetPercent(0);
	GenerationProgressBar->SetVisibility(ESlateVisibility::Hidden);
	
	const TArray<AUnit*> SelectedUnits = SelectionManager->GetSelectedUnits();
	const TArray<ABuilding*> SelectedBuildings = SelectionManager->GetSelectedBuildings();
	const ESelectionState SelectionState = SelectionManager->GetSelectionState();
	
	if(SelectionState == ESelectionState::BuildingSingle)
	{
		if(SelectedBuildings.Num() != 1)
		{
			UE_LOG(LogTemp,Error,TEXT("Selection State Does not match with selection"));
			return;
		}

		const ABuilding* SelectedBuilding = SelectedBuildings[0];
		const EBuildingState BuildingState = SelectedBuilding->GetBuildingState();

		if(BuildingState == EBuildingState::Located)
		{
			UUnitGenerator* UnitGenerator = SelectedBuilding->FindComponentByClass<UUnitGenerator>();
			if(UnitGenerator)
			{
				const TArray<FUnitData> UnitQueue = UnitGenerator->GetUnitQueue();
				if(UnitQueue.Num() > 0)
				{
					GenerationProgressBar->SetVisibility(ESlateVisibility::Visible);
					GenerationProgressBar->SetPercent(UnitGenerator->GetProgress());
				}
			}
		}
	}
}

void UBuildingWidget::OnUnitQueueUpdated()
{
	UnitQueueTileView->ClearListItems();
	UnitQueueTileView->SetVisibility(ESlateVisibility::Hidden);
	
	const TArray<AUnit*> SelectedUnits = SelectionManager->GetSelectedUnits();
	const TArray<ABuilding*> SelectedBuildings = SelectionManager->GetSelectedBuildings();
	const ESelectionState SelectionState = SelectionManager->GetSelectionState();
	
	if(SelectionState == ESelectionState::BuildingSingle)
	{
		if(SelectedBuildings.Num() != 1)
		{
			UE_LOG(LogTemp,Error,TEXT("Selection State Does not match with selection"));
			return;
		}

		const ABuilding* SelectedBuilding = SelectedBuildings[0];
		const EBuildingState BuildingState = SelectedBuilding->GetBuildingState();

		if(BuildingState == EBuildingState::Located)
		{
			UUnitGenerator* UnitGenerator = SelectedBuilding->FindComponentByClass<UUnitGenerator>();
			if(UnitGenerator)
			{
				TArray<FUnitData> UnitQueue = UnitGenerator->GetUnitQueue();
				if(UnitQueue.Num() > 0)
				{
					CreateQueueEntries(UnitQueue,UnitGenerator);
					UnitQueueTileView->SetVisibility(ESlateVisibility::Visible);
				}
			}
		}
	}
}

void UBuildingWidget::CreateUnitEntries(TArray<FUnitData>& UnitsData, UUnitGenerator* UnitGenerator) const
{
	for(const FUnitData &UnitData : UnitsData)
	{
		UUnitEntryArgument* UnitEntryArgument =
			NewObject<UUnitEntryArgument>();
		
		UnitEntryArgument->UnitData = UnitData;
		UnitEntryArgument->UnitGenerator = UnitGenerator;
		
		UnitsTileView->AddItem(UnitEntryArgument);
	}

	UnitsTileView->SetVisibility(ESlateVisibility::Visible);
	PanelBackground->SetVisibility(ESlateVisibility::Visible);
}

void UBuildingWidget::CreateQueueEntries(TArray<FUnitData>& UnitQueue, UUnitGenerator* UnitGenerator) const
{
	for(const FUnitData &UnitData : UnitQueue)
	{
		UUnitQueueEntryArgument* UnitQueueEntryArgument =
			NewObject<UUnitQueueEntryArgument>();
		
		UnitQueueEntryArgument->UnitData = UnitData;
		UnitQueueEntryArgument->UnitGenerator = UnitGenerator;
		UnitQueueEntryArgument->Size = 1;
		UnitQueueEntryArgument->QueueIndex = -1;
		
		UnitQueueTileView->AddItem(UnitQueueEntryArgument);
	}
}

void UBuildingWidget::HardReset()
{
	UnitsTileView->ClearListItems();
	UnitsTileView->SetVisibility(ESlateVisibility::Hidden);
	PanelBackground->SetVisibility(ESlateVisibility::Hidden);

	const TArray<AUnit*> SelectedUnits = SelectionManager->GetSelectedUnits();
	const TArray<ABuilding*> SelectedBuildings = SelectionManager->GetSelectedBuildings();
	const ESelectionState SelectionState = SelectionManager->GetSelectionState();
	
	if(SelectionState == ESelectionState::BuildingSingle)
	{
		if(SelectedBuildings.Num() != 1)
		{
			UE_LOG(LogTemp,Error,TEXT("Selection State Does not match with selection"));
			return;
		}

		const ABuilding* SelectedBuilding = SelectedBuildings[0];
		const EBuildingState BuildingState = SelectedBuilding->GetBuildingState();

		if(BuildingState == EBuildingState::Located)
		{
			UUnitGenerator* UnitGenerator = SelectedBuilding->FindComponentByClass<UUnitGenerator>();
			if(UnitGenerator)
			{
				CreateUnitEntries(UnitGenerator->GetUnits(),UnitGenerator);
			}
		}
	}


	OnUnitGenerationProgressUpdated();
	OnUnitQueueUpdated();
}



