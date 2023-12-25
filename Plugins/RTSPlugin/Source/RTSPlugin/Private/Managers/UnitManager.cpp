// Fill out your copyright notice in the Description page of Project Settings.


#include "Managers/UnitManager.h"

#include "ActorsAndComponents/Unit.h"
#include "Managers/PlayerManager.h"
#include "Managers/RTSGameInstance.h"
#include "Managers/SelectionManager.h"

void UUnitManager::Init(URTSGameInstance* gameInstance)
{
	Super::Init(gameInstance);

	PlayerManager = GameInstance->PlayerManager;
	SelectionManager = GameInstance->SelectionManager;

	PlayerManager->OnMouseRightClickDelegate.AddUniqueDynamic(this,&UUnitManager::OnMouseRightClicked);
}

void UUnitManager::Begin()
{
	Super::Begin();
}

void UUnitManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void UUnitManager::OnMouseRightClicked()
{
	TArray<AUnit*> SelectedUnits = SelectionManager->GetSelectedUnits();

	//Case 1 -> Hit RTS Entity, Set Target Entity
	if(const URTSEntity* Entity = PlayerManager->LookForRTSEntity())
	{
		for(AUnit* SelectedUnit : SelectedUnits)
		{
			SelectedUnit->SetTargetRTSEntity(Entity);
		}
		return;
	}
	
	//Case 2 -> Hit Floor, Set Target Location
	const FHitResult Hit = PlayerManager->LookForFloor();
	for(AUnit* SelectedUnit : SelectedUnits)
	{
		SelectedUnit->SetTargetLocation(Hit.Location);
	}
}
