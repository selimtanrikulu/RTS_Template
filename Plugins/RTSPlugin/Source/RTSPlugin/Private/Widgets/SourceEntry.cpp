// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/SourceEntry.h"

#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Kismet/GameplayStatics.h"
#include "Managers/RTSGameInstance.h"
#include "Managers/SourceManager.h"

void USourceEntry::NativeConstruct()
{
	Super::NativeConstruct();

	const APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(),0);
	const URTSGameInstance* GameInstance = Cast<URTSGameInstance>(PlayerController->GetGameInstance());
	SourceManager = GameInstance->SourceManager;
	
}

void USourceEntry::NativeDestruct()
{
	Super::NativeDestruct();

	SourceManager->GetSourceChangeDelegate(SourceEntryArgument->SourceInfo.Name).
	RemoveDynamic(this,&USourceEntry::OnSourceUpdated);
}

void USourceEntry::StartEntry(USourceEntryArgument* EntryArgument)
{
	SourceEntryArgument = EntryArgument;

	SourceNameText->SetText(FText::FromString(SourceEntryArgument->SourceInfo.Name));
	
	if(SourceEntryArgument->SourceInfo.SourceImage)
	{
		SourceImage->SetBrushFromTexture(SourceEntryArgument->SourceInfo.SourceImage);
	}
	else
	{
		UE_LOG(LogTemp,Warning,TEXT("Image material not exist"));
	}
	
	SourceManager->GetSourceChangeDelegate(SourceEntryArgument->SourceInfo.Name).
	AddUniqueDynamic(this,&USourceEntry::OnSourceUpdated);

	UpdateAmount();
}

void USourceEntry::OnSourceUpdated()
{
	UpdateAmount();
}


void USourceEntry::UpdateAmount() const
{
	const int Amount = SourceManager->GetSourceAmount(SourceEntryArgument->SourceInfo.Name);
	SourceAmountText->SetText(FText::AsNumber(Amount));
}



