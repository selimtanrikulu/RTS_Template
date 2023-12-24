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
}
