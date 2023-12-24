// Fill out your copyright notice in the Description page of Project Settings.


#include "..\..\Public\Managers\SelectionManager.h"

#include "Managers/PlayerManager.h"
#include "ActorsAndComponents/SelectBox.h"
#include "Managers/LogManager.h"
#include "Managers/RTSGameInstance.h"
#include "Utility/Util.h"
#include "ActorsAndComponents/Building.h"
#include "ActorsAndComponents/SourceHolder.h"
#include "ActorsAndComponents/TeamEntity.h"
#include "ActorsAndComponents/NeutralEntity.h"
#include "ActorsAndComponents/Unit.h"
#include "Managers/Asset_Manager.h"

USelectionManager::USelectionManager()
{
}

USelectionManager::~USelectionManager()
{
}

void USelectionManager::Init(URTSGameInstance* gameInstance)
{
	Super::Init(gameInstance);

	//Get dependencies
    LogManager = GameInstance->LogManager;
    AssetManager = GameInstance->AssetManager;
	PlayerManager = GameInstance->PlayerManager;
    //

	
	PlayerManager->OnMouseLeftClickDelegate.AddUniqueDynamic(this,&USelectionManager::OnMouseLeftClicked);
	PlayerManager->OnMouseLeftReleasedDelegate.AddUniqueDynamic(this,&USelectionManager::OnMouseLeftReleased);
	PlayerManager->OnMouseRightClickDelegate.AddUniqueDynamic(this,&USelectionManager::OnMouseRightClicked);
	PlayerManager->OnDeleteClickDelegate.AddUniqueDynamic(this,&USelectionManager::OnDeleteClicked);
}

void USelectionManager::Begin()
{
	Super::Begin();
}


void USelectionManager::Tick(float DeltaTime)
{
	UpdateCircles();
}

void USelectionManager::OnMouseLeftClicked()
{
	if(CurrentSelectBox)
	{
		LogManager->EditorLog
		(FText::FromString(TEXT("Select box already exist")),ELogVerbosity::Warning);
		return;
	}
	
	CreateSelectBox();
}

void USelectionManager::OnMouseLeftReleased()
{
	if(!CurrentSelectBox)
	{
		LogManager->EditorLog
		(FText::FromString(TEXT("Select box does not exist")),ELogVerbosity::Warning);
		return;
	}
		
	DestroySelectBox();
}

void USelectionManager::OnMouseRightClicked()
{
	const FHitResult Hit = PlayerManager->LookForFloor();
	
	for(AUnit* SelectedUnit : SelectedUnits)
	{
		SelectedUnit->SetTargetLocation(Hit.Location);
	}
}


void USelectionManager::OnDeleteClicked()
{
	for(ABuilding* Building : SelectedBuildings)
	{
		GameInstance->World->DestroyActor(Building);
	}
	for(AUnit* Unit : SelectedUnits)
	{
		GameInstance->World->DestroyActor(Unit);
	}

	SelectedBuildings.Empty();
	SelectedUnits.Empty();

	UpdateSelectionState();

	OnSelectionChangedDelegate.Broadcast();
}

void USelectionManager::OnOverlapChanged()
{
	//Update selection
	UnbindSelections();
	SelectedBuildings = CurrentSelectBox->OverlappingBuildings;
	SelectedUnits = CurrentSelectBox->OverlappingUnits;
	SelectedSourceHolders = CurrentSelectBox->OverlappingSourceHolders;
	BindSelections();
	
	UpdateSelectionState();
	
	//Fire main delegate
	OnSelectionChangedDelegate.Broadcast();
}


void USelectionManager::UpdateCircles()
{
	for(const ABuilding* SelectedBuilding : SelectedBuildings)
	{
		DrawCircle(GameInstance->World,
		SelectedBuilding->TeamEntity->MeshComponent->GetComponentLocation(),
		FVector::RightVector,
		FVector::ForwardVector,
		FColor::Black,
		SelectedBuilding->TeamEntity->Extent,
		300,
		false,
		-1,
		0,
		20
		);
	}


	for(const AUnit* SelectedUnit : SelectedUnits)
	{
		DrawCircle(GameInstance->World,
		SelectedUnit->TeamEntity->MeshComponent->GetComponentLocation(),
		FVector::RightVector,
		FVector::ForwardVector,
		FColor::Blue,
		SelectedUnit->TeamEntity->Extent,
		300,
		false,
		-1,
		0,
		5
		);
	}

	for(const USourceHolder* SourceHolder : SelectedSourceHolders)
	{
		DrawCircle(GameInstance->World,
		SourceHolder->NeutralEntity->MeshComponent->GetComponentLocation(),
		FVector::RightVector,
		FVector::ForwardVector,
		FColor::White,
		SourceHolder->NeutralEntity->Extent,
		300,
		false,
		-1,
		0,
		5
		);
	}

}

void USelectionManager::UpdateSelectionState()
{
	if(SelectedUnits.Num() == 0)
	{
		if(SelectedBuildings.Num() == 0)
		{
			SelectionState = ESelectionState::None;
		}
		else if(SelectedBuildings.Num() == 1)
		{
			SelectionState = ESelectionState::BuildingSingle;
		}
		else
		{
			if(AreSameBuildings(SelectedBuildings))
			{
				SelectionState = ESelectionState::BuildingMass;
			}
			else
			{
				SelectionState = ESelectionState::BuildingMixed;
			}
			
		}
	}
	else if(SelectedUnits.Num() == 1)
	{
		if(SelectedBuildings.Num() == 0)
		{
			SelectionState = ESelectionState::UnitSingle;
		}
		else
		{
			SelectionState = ESelectionState::AllMixed;
		}
	}
	else
	{
		if(SelectedBuildings.Num() == 0)
		{
			if(AreSameUnits(SelectedUnits))
			{
				SelectionState = ESelectionState::UnitMass;
			}
			else
			{
				SelectionState = ESelectionState::UnitMixed;
			}
		}
		else
		{
			SelectionState = ESelectionState::AllMixed;
		}
	}
}

void USelectionManager::OnBuildingChanged(ABuilding* Building)
{
	OnSelectionChangedDelegate.Broadcast();
}

void USelectionManager::OnEntityKilled(URTSEntity* Entity)
{
	//Remove
	SelectedBuildings.RemoveAll([&](const ABuilding* Element)
		{ return Element->TeamEntity == Entity; });
	
	SelectedUnits.RemoveAll([&](const AUnit* Element)
		{ return Element->TeamEntity == Entity; });

	UpdateSelectionState();
	OnSelectionChangedDelegate.Broadcast();
}

void USelectionManager::OnTeamEntityGetDamage(UTeamEntity* TeamEntity)
{
	OnSelectionChangedDelegate.Broadcast();
}

void USelectionManager::BindSelections()
{
	for(ABuilding* Building : SelectedBuildings)
	{
		Building->OnBuildingChangedDelegate.AddUniqueDynamic(this,&USelectionManager::OnBuildingChanged);
		Building->TeamEntity->OnEntityKilledDelegate.AddUniqueDynamic(this,&USelectionManager::OnEntityKilled);
		Building->TeamEntity->OnTeamEntityGetDamageDelegate.AddUniqueDynamic(this,&USelectionManager::OnTeamEntityGetDamage);
	}
	for(const AUnit* Unit : SelectedUnits)
	{
		Unit->TeamEntity->OnEntityKilledDelegate.AddUniqueDynamic(this,&USelectionManager::OnEntityKilled);
		Unit->TeamEntity->OnTeamEntityGetDamageDelegate.AddUniqueDynamic(this,&USelectionManager::OnTeamEntityGetDamage);
	}
}

void USelectionManager::UnbindSelections()
{
	for(ABuilding* Building : SelectedBuildings)
	{
		Building->OnBuildingChangedDelegate.RemoveDynamic(this,&USelectionManager::OnBuildingChanged);
		Building->TeamEntity->OnEntityKilledDelegate.RemoveDynamic(this,&USelectionManager::OnEntityKilled);
		Building->TeamEntity->OnTeamEntityGetDamageDelegate.RemoveDynamic(this,&USelectionManager::OnTeamEntityGetDamage);
	}
	for(const AUnit* Unit : SelectedUnits)
	{
		Unit->TeamEntity->OnEntityKilledDelegate.RemoveDynamic(this,&USelectionManager::OnEntityKilled);
		Unit->TeamEntity->OnTeamEntityGetDamageDelegate.RemoveDynamic(this,&USelectionManager::OnTeamEntityGetDamage);
	}
}


void USelectionManager::CreateSelectBox()
{
	const FHitResult Hit = PlayerManager->LookForFloor();

	const FVector Location = Hit.Location;
	const FRotator Rotation(0);
	
	AActor* SpawnedActor = GameInstance->World->SpawnActor<AActor>(AssetManager->GetSelectBoxBP(),Location,Rotation);
	CurrentSelectBox = Cast<ASelectBox>(SpawnedActor);
	
	//Load overlaps to select box
	CurrentSelectBox->OverlappingBuildings = SelectedBuildings;
	CurrentSelectBox->OverlappingUnits = SelectedUnits;
	CurrentSelectBox->OverlappingSourceHolders = SelectedSourceHolders;
	
	CurrentSelectBox->OnOverlapChangedDelegate.AddUniqueDynamic(this,&USelectionManager::OnOverlapChanged);
}

void USelectionManager::DestroySelectBox()
{
	CurrentSelectBox->OnOverlapChangedDelegate.RemoveDynamic(this,&USelectionManager::OnOverlapChanged);
	GameInstance->World->DestroyActor(CurrentSelectBox);
	CurrentSelectBox = nullptr;
}

TArray<ABuilding*> USelectionManager::GetSelectedBuildings() const
{
	return SelectedBuildings;
}

TArray<AUnit*> USelectionManager::GetSelectedUnits() const
{
	return SelectedUnits;
}

TArray<USourceHolder*> USelectionManager::GetSelectedSourceHolders() const
{
	return SelectedSourceHolders;
}

TArray<UTeamEntity*> USelectionManager::GetSelectedTeamEntities() const
{
	TArray<UTeamEntity*> SelectedTeamEntities;

	for(const ABuilding* Building : SelectedBuildings)
	{
		SelectedTeamEntities.Add(Building->TeamEntity);
	}
	for(const AUnit* Unit : SelectedUnits)
	{
		SelectedTeamEntities.Add(Unit->TeamEntity);
	}

	return SelectedTeamEntities;
}

TArray<URTSEntity*> USelectionManager::GetSelectedRTSEntities() const
{
	TArray<URTSEntity*> SelectedRTSEntities;

	for(const ABuilding* Building : SelectedBuildings)
	{
		SelectedRTSEntities.Add(Building->TeamEntity);
	}
	for(const AUnit* Unit : SelectedUnits)
	{
		SelectedRTSEntities.Add(Unit->TeamEntity);
	}
	for(const USourceHolder* SourceHolder : SelectedSourceHolders)
	{
		SelectedRTSEntities.Add(SourceHolder->NeutralEntity);
	}

	return SelectedRTSEntities;
}

ESelectionState USelectionManager::GetSelectionState() const
{
	return SelectionState;
}


bool USelectionManager::AreSameBuildings(TArray<ABuilding*> Buildings)
{
	if(Buildings.Num() == 0)return true;

	const int BuildingID = Buildings[0]->GetBuildingData().TeamEntityData.EntityData.EntityID;
	for(const ABuilding* Building : Buildings)
	{
		if(BuildingID != Building->GetBuildingData().TeamEntityData.EntityData.EntityID)
		{
			return false;
		}
	}

	return true;
}

bool USelectionManager::AreSameUnits(TArray<AUnit*> Units)
{
	if(Units.Num() == 0)return true;

	const int UnitID = Units[0]->GetUnitData().TeamEntityData.EntityData.EntityID;
	for(const AUnit* Unit : Units)
	{
		if(UnitID != Unit->GetUnitData().TeamEntityData.EntityData.EntityID)
		{
			return false;
		}
	}

	return true;
}
