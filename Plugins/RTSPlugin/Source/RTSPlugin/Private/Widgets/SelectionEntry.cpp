// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/SelectionEntry.h"

#include "Components/Button.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Kismet/GameplayStatics.h"
#include "Managers/BuildingManager.h"
#include "Managers/RTSGameInstance.h"
#include "Widgets/BuildingEntry.h"

void USelectionEntry::NativeConstruct()
{
	Super::NativeConstruct();

	const APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(),0);
	const URTSGameInstance* GameInstance = Cast<URTSGameInstance>(PlayerController->GetGameInstance());

	BuildingManager = GameInstance->BuildingManager;

	EntryButton->OnClicked.AddUniqueDynamic(this,&USelectionEntry::OnEntryButtonClicked);
}

void USelectionEntry::StartEntry(USelectionEntryArgument* selectionEntryArgument)
{
	SelectionEntryArgument = selectionEntryArgument;
	
	SelectionNameText->SetText(FText::FromString(SelectionEntryArgument->EntityData.Name));
	SelectionSizeText->SetText(FText::AsNumber(SelectionEntryArgument->Size));


	if(SelectionEntryArgument->EntityData.ImageMaterial)
	{
		SelectionImage->SetBrushFromMaterial(Cast<UMaterialInterface>
		(SelectionEntryArgument->EntityData.ImageMaterial));
	}
	else
	{
		UE_LOG(LogTemp,Warning,TEXT("Image material not exist"));
	}
}

void USelectionEntry::OnEntryButtonClicked()
{
	//TODO : Select That/Those Entries
}
