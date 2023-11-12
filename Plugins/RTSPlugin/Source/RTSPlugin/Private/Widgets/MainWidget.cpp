// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/MainWidget.h"

#include "Components/TileView.h"
#include "Kismet/GameplayStatics.h"
#include "Managers/BuildingManager.h"
#include "Managers/RTSGameInstance.h"
#include "Managers/USelectionManager.h"
#include "Widgets/BuildingEntry.h"

void UMainWidget::NativeConstruct()
{
	Super::NativeConstruct();

	const APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(),0);
	const URTSGameInstance* GameInstance = Cast<URTSGameInstance>(PlayerController->GetGameInstance());

	SelectionManager = GameInstance->SelectionManager;
	BuildingManager = GameInstance->BuildingManager;
	
	SelectionManager->OnSelectionChangedDelegate.AddUniqueDynamic(this,&UMainWidget::OnSelectionChanged);
	
	TArray<FBuildingData> BuildingsData = BuildingManager->GetBuildingsData();
	for(const FBuildingData &BuildingData : BuildingsData)
	{
		UBuildingEntryArgument* BuildingEntryArgument =
			NewObject<UBuildingEntryArgument>();
		
		BuildingEntryArgument->BuildingData = BuildingData;
		BuildingsTileView->AddItem(BuildingEntryArgument);
	}
	
}

void UMainWidget::OnSelectionChanged()
{
	const TArray<ABuilding*> SelectedBuildings = SelectionManager->GetSelectedBuildings();
	UE_LOG(LogTemp,Display,TEXT("Selection Changed. Buildings : %d"),SelectedBuildings.Num());
}
