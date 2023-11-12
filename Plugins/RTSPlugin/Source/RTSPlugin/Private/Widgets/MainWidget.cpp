// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/MainWidget.h"

#include "Components/TileView.h"
#include "Kismet/GameplayStatics.h"
#include "Managers/BuildingManager.h"
#include "Managers/RTSGameInstance.h"
#include "Managers/StoreManager.h"
#include "Managers/USelectionManager.h"
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
	
	TArray<FBuildingData> BuildingsData = StoreManager->GetCurrentBuildings();
	for(const FBuildingData &BuildingData : BuildingsData)
	{
		UBuildingEntryArgument* BuildingEntryArgument =
			NewObject<UBuildingEntryArgument>();
		
		BuildingEntryArgument->BuildingData = BuildingData;
		BuildingsTileView->AddItem(BuildingEntryArgument);
	}



	TArray<FUnitData> UnitsData = StoreManager->GetCurrentUnits();
	for(const FUnitData &UnitData : UnitsData)
	{
		UUnitEntryArgument* UnitEntryArgument =
			NewObject<UUnitEntryArgument>();
		
		UnitEntryArgument->UnitData = UnitData;
		UnitsTileView->AddItem(UnitEntryArgument);
	}
	
}

void UMainWidget::OnSelectionChanged()
{
	const TArray<ABuilding*> SelectedBuildings = SelectionManager->GetSelectedBuildings();
	const TArray<AUnit*> SelectedUnits = SelectionManager->GetSelectedUnits();

	
	UE_LOG(LogTemp,Display,TEXT("Selection Changed. Buildings : %d Units %d"),
		SelectedBuildings.Num(),
		SelectedUnits.Num());
}
