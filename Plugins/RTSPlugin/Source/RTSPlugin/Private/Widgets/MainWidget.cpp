// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/MainWidget.h"

#include "ActorsAndComponents/Building.h"
#include "ActorsAndComponents/UnitGenerator.h"
#include "ActorsAndComponents/Worker.h"
#include "Components/TileView.h"
#include "Kismet/GameplayStatics.h"
#include "Managers/RTSGameInstance.h"
#include "Managers/StoreManager.h"
#include "..\..\Public\Managers\SelectionManager.h"
#include "Components/ProgressBar.h"
#include "Widgets/BuildingEntry.h"
#include "Widgets/UnitEntry.h"

void UMainWidget::NativeConstruct()
{
	Super::NativeConstruct();

	const APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(),0);
	const URTSGameInstance* GameInstance = Cast<URTSGameInstance>(PlayerController->GetGameInstance());

	SelectionManager = GameInstance->SelectionManager;
	StoreManager = GameInstance->StoreManager;
	
	SelectionManager->OnSelectionChangedDelegate.AddUniqueDynamic(this,&UMainWidget::OnSelectionChanged);
}

void UMainWidget::OnSelectionChanged()
{
	BuildingsTileView->ClearListItems();
	UnitsTileView->ClearListItems();

	BuildingsTileView->SetVisibility(ESlateVisibility::Hidden);
	UnitsTileView->SetVisibility(ESlateVisibility::Hidden);
	

	const TArray<ABuilding*> SelectedBuildings = SelectionManager->GetSelectedBuildings();
	const TArray<AUnit*> SelectedUnits = SelectionManager->GetSelectedUnits();
	const ESelectionState SelectionState = SelectionManager->GetSelectionState();

	ProgressBar->SetVisibility(ESlateVisibility::Hidden);

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

		ProgressBar->SetVisibility(ESlateVisibility::Visible);
		ProgressBar->SetPercent(SelectedBuilding->GetProgress());
	}
	else if(SelectionState == ESelectionState::UnitSingle ||
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

			TArray<FBuildingData> BuildingsData = StoreManager->GetCurrentBuildings();
			CreateBuildingEntries(BuildingsData);
		}
		
	}
	
	
	
	UE_LOG(LogTemp,Display,TEXT("Selection Changed. Buildings : %d Units %d"),
		SelectedBuildings.Num(),
		SelectedUnits.Num());


	UE_LOG(LogTemp,Display,TEXT("Selection State : %s"),
		*UEnum::GetValueAsString(SelectionState));
}

void UMainWidget::CreateUnitEntries(TArray<FUnitData> &UnitsData,UUnitGenerator* UnitGenerator) const
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
}

void UMainWidget::CreateBuildingEntries(TArray<FBuildingData> &BuildingsData) const
{
	for(const FBuildingData &BuildingData : BuildingsData)
	{
		UBuildingEntryArgument* BuildingEntryArgument =
			NewObject<UBuildingEntryArgument>();
		
		BuildingEntryArgument->BuildingData = BuildingData;
		BuildingsTileView->AddItem(BuildingEntryArgument);
	}

	BuildingsTileView->SetVisibility(ESlateVisibility::Visible);
}
