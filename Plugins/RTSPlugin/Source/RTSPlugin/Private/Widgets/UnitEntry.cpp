// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/UnitEntry.h"

#include "Components/Button.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Kismet/GameplayStatics.h"
#include "Managers/RTSGameInstance.h"
#include "Widgets/BuildingEntry.h"


void UUnitEntry::NativeConstruct()
{
	Super::NativeConstruct();

	const APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(),0);
	const URTSGameInstance* GameInstance = Cast<URTSGameInstance>(PlayerController->GetGameInstance());


	EntryButton->OnClicked.AddUniqueDynamic(this,&UUnitEntry::OnEntryButtonClicked);
}

void UUnitEntry::StartEntry(UUnitEntryArgument* unitEntryArgument)
{
	UnitEntryArgument = unitEntryArgument;

	

	UnitNameText->SetText(FText::FromString(unitEntryArgument->UnitData.TeamEntityData.EntityData.Name));


	if(UnitEntryArgument->UnitData.TeamEntityData.EntityData.ImageMaterial)
	{
		UnitImage->SetBrushFromMaterial(Cast<UMaterialInterface>
		(UnitEntryArgument->UnitData.TeamEntityData.EntityData.ImageMaterial));
	}
	else
	{
		UE_LOG(LogTemp,Warning,TEXT("Image material not exist"));
	}
}

void UUnitEntry::OnEntryButtonClicked()
{
	UnitEntryArgument->UnitGenerator->SpawnUnit(UnitEntryArgument->UnitData);
}
