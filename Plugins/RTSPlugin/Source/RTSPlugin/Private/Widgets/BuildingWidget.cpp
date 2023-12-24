// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/BuildingWidget.h"

#include "ActorsAndComponents/Building.h"
#include "ActorsAndComponents/UnitGenerator.h"
#include "Components/ProgressBar.h"
#include "Components/TileView.h"
#include "Kismet/GameplayStatics.h"
#include "Managers/RTSGameInstance.h"
#include "Managers/SelectionManager.h"
#include "Widgets/BuildingEntry.h"
#include "Widgets/UnitEntry.h"


void UBuildingWidget::NativeConstruct()
{
	Super::NativeConstruct();

	const APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(),0);
	const URTSGameInstance* GameInstance = Cast<URTSGameInstance>(PlayerController->GetGameInstance());

	ProgressBar->SetVisibility(ESlateVisibility::Hidden);
	
	SelectionManager = GameInstance->SelectionManager;
	StoreManager = GameInstance->StoreManager;
	
	SelectionManager->OnSelectionChangedDelegate.AddUniqueDynamic(this,&UBuildingWidget::OnSelectionChanged);
}

void UBuildingWidget::OnSelectionChanged()
{
	UnitsTileView->ClearListItems();
	UnitsTileView->SetVisibility(ESlateVisibility::Hidden);
	ProgressBar->SetVisibility(ESlateVisibility::Hidden);

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

		ProgressBar->SetVisibility(ESlateVisibility::Visible);
		ProgressBar->SetPercent(SelectedBuilding->GetProgress());
	}

	
	UE_LOG(LogTemp,Display,TEXT("Selection Changed. Buildings : %d Units %d"),
		SelectedBuildings.Num(),
		SelectedUnits.Num());


	UE_LOG(LogTemp,Display,TEXT("Selection State : %s"),
		*UEnum::GetValueAsString(SelectionState));
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
}

