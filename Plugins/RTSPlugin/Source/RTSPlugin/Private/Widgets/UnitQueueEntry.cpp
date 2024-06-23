// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/UnitQueueEntry.h"

#include "ActorsAndComponents/UnitGenerator.h"
#include "Components/Button.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"


void UUnitQueueEntry::NativeConstruct()
{
	Super::NativeConstruct();
	
	EntryButton->OnClicked.AddUniqueDynamic(this,&UUnitQueueEntry::OnEntryButtonClicked);
}

void UUnitQueueEntry::StartEntry(UUnitQueueEntryArgument* unitQueueEntryArgument)
{
	UnitQueueEntryArgument = unitQueueEntryArgument;
	
	UnitNameText->SetText(FText::FromString(UnitQueueEntryArgument->UnitData.TeamEntityData.EntityData.Name));


	if(UnitQueueEntryArgument->UnitData.TeamEntityData.EntityData.ImageMaterial)
	{
		UnitImage->SetBrushFromMaterial(Cast<UMaterialInterface>
		(UnitQueueEntryArgument->UnitData.TeamEntityData.EntityData.ImageMaterial));
	}
	else
	{
		UE_LOG(LogTemp,Warning,TEXT("Image material not exist"));
	}
}

void UUnitQueueEntry::OnEntryButtonClicked()
{
	UnitQueueEntryArgument->UnitGenerator->PopUnitFromQueue(UnitQueueEntryArgument->QueueIndex);
}
