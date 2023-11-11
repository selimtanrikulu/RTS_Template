// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/BuildingEntry.h"

#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Kismet/GameplayStatics.h"
#include "Managers/BuildingManager.h"
#include "Managers/RTSGameInstance.h"


void UBuildingEntry::NativeConstruct()
{
	Super::NativeConstruct();

	const APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(),0);
	const URTSGameInstance* GameInstance = Cast<URTSGameInstance>(PlayerController->GetGameInstance());


	BuildingManager = GameInstance->BuildingManager;

	EntryButton->OnClicked.AddUniqueDynamic(this,&UBuildingEntry::OnEntryButtonClicked);
}

void UBuildingEntry::StartEntry(UBuildingEntryArgument* buildingEntryArgument)
{
	BuildingEntryArgument = buildingEntryArgument;
	BuildingNameText->SetText(FText::FromString(buildingEntryArgument->BuildingData.Name));
}

void UBuildingEntry::OnEntryButtonClicked()
{
	BuildingManager->DraftBuilding(BuildingEntryArgument->BuildingData);
}
