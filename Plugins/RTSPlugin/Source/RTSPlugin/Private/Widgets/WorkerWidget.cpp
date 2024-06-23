// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/WorkerWidget.h"
#include "ActorsAndComponents/Worker.h"
#include "Components/Image.h"
#include "Components/TileView.h"
#include "Kismet/GameplayStatics.h"
#include "Managers/RTSGameInstance.h"
#include "Managers/SelectionManager.h"
#include "Managers/StoreManager.h"
#include "Widgets/BuildingEntry.h"


void UWorkerWidget::NativeConstruct()
{
	Super::NativeConstruct();

	const APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(),0);
	const URTSGameInstance* GameInstance = Cast<URTSGameInstance>(PlayerController->GetGameInstance());

	SelectionManager = GameInstance->SelectionManager;
	StoreManager = GameInstance->StoreManager;
	
	SelectionManager->OnSelectedEntitiesChangedDelegate.AddUniqueDynamic(this,&UWorkerWidget::OnSelectionChanged);

	ClearWidget();
}

void UWorkerWidget::OnSelectionChanged()
{
	ClearWidget();
	UpdateWidget();	
}


void UWorkerWidget::CreateBuildingEntries() const
{
	TArray<FBuildingData> BuildingsData = StoreManager->GetCurrentBuildings();
	
	for(const FBuildingData &BuildingData : BuildingsData)
	{
		UBuildingEntryArgument* BuildingEntryArgument =
			NewObject<UBuildingEntryArgument>();
		
		BuildingEntryArgument->BuildingData = BuildingData;
		BuildingsTileView->AddItem(BuildingEntryArgument);
	}

	BuildingsTileView->SetVisibility(ESlateVisibility::Visible);
	PanelBackground->SetVisibility(ESlateVisibility::Visible);
}

void UWorkerWidget::UpdateWidget() const
{
	const TArray<AUnit*> SelectedUnits = SelectionManager->GetSelectedUnits();
	const ESelectionState SelectionState = SelectionManager->GetSelectionState();
	
	if(SelectionState == ESelectionState::UnitSingle ||
		SelectionState == ESelectionState::UnitMass)
	{
		if(SelectedUnits.Num() == 0)
		{
			UE_LOG(LogTemp,Error,TEXT("Selection State Does not match with selection"));
			return;
		}

		AUnit* Unit = SelectedUnits[0];
		const AWorker* Worker = Cast<AWorker>(Unit);

		if(Worker)
		{
			StoreManager->OpenWorkerRoot();
			CreateBuildingEntries();
		}
		
	}
	
	UE_LOG(LogTemp,Display,TEXT("Selection Changed. Units %d"),
		SelectedUnits.Num());


	UE_LOG(LogTemp,Display,TEXT("Selection State : %s"),
		*UEnum::GetValueAsString(SelectionState));
}

void UWorkerWidget::ClearWidget() const
{
	BuildingsTileView->ClearListItems();
	BuildingsTileView->SetVisibility(ESlateVisibility::Hidden);

	PanelBackground->SetVisibility(ESlateVisibility::Hidden);
}
