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
#include "ActorsAndComponents/Source.h"
#include "ActorsAndComponents/Unit.h"
#include "ActorsAndComponents/UnitGenerator.h"
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
	PlayerManager->OnDeleteClickDelegate.AddUniqueDynamic(this,&USelectionManager::OnDeleteClicked);
}
void USelectionManager::Begin()
{
	Super::Begin();
}
void USelectionManager::Tick(float DeltaTime)
{
	//UpdateCircles();
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
		return;
	}
		
	DestroySelectBox();
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
}
void USelectionManager::OnOverlapChanged()
{
	//Update selection
	UnbindSelections();
	SelectedBuildings = CurrentSelectBox->OverlappingBuildings;
	SelectedUnits = CurrentSelectBox->OverlappingUnits;
	SelectedSources = CurrentSelectBox->OverlappingSources;
	BindSelections();
	UpdateSelectionState();
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

	for(const ASource* Source : SelectedSources)
	{
		DrawCircle(GameInstance->World,
		Source->NeutralEntity->MeshComponent->GetComponentLocation(),
		FVector::RightVector,
		FVector::ForwardVector,
		FColor::White,
		Source->NeutralEntity->Extent,
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
		if(SelectedSources.Num() == 0)
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
		else if(SelectedSources.Num() == 1)
		{
			if(SelectedBuildings.Num() == 0)
			{
				SelectionState = ESelectionState::SourceSingle;
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
				if(AreSameSources(SelectedSources))
				{
					SelectionState = ESelectionState::SourceMass;
				}
				else
				{
					SelectionState = ESelectionState::SourceMixed;
				}
			}
			else
			{
				SelectionState = ESelectionState::AllMixed;
			}
		}
		
	}
	else if(SelectedUnits.Num() == 1)
	{
		if(SelectedSources.Num() == 0)
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
			SelectionState = ESelectionState::AllMixed;
		}
	}
	else
	{
		if(SelectedSources.Num() == 0)
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
		else
		{
			SelectionState = ESelectionState::AllMixed;
		}
		
	}
	
	
	OnSelectedEntitiesChangedDelegate.Broadcast();
}
void USelectionManager::OnTeamEntityKilled(UTeamEntity* TeamEntity)
{
	//Remove
	SelectedBuildings.RemoveAll([&](const ABuilding* Element)
		{ return Element->TeamEntity == TeamEntity; });
	
	SelectedUnits.RemoveAll([&](const AUnit* Element)
		{ return Element->TeamEntity == TeamEntity; });
	
	
	UpdateSelectionState();
}
void USelectionManager::OnTeamEntityHPChanged(UTeamEntity* TeamEntity)
{
	OnTeamEntityHPChangedDelegate.Broadcast(TeamEntity);
}

void USelectionManager::OnBuildingStateChanged(ABuilding* Building)
{
	OnBuildingStateChangedDelegate.Broadcast(Building);
}

void USelectionManager::OnSourceCollected(USourceHolder* SourceHolder)
{
	OnSourceCollectedDelegate.Broadcast(SourceHolder);
}
void USelectionManager::OnSourceFinished(USourceHolder* SourceHolder)
{
	SelectedSources.RemoveAll([&](const ASource* Element)
		{ return Element->SourceHolder == SourceHolder; });
	
	SelectedBuildings.RemoveAll([&](const ABuilding* Element)
		{ return Element->FindComponentByClass<USourceHolder>() == SourceHolder; });
	
	UpdateSelectionState();
}

void USelectionManager::OnUnitGenerationProgressUpdated()
{
	OnUnitGenerationProgressUpdatedDelegate.Broadcast();
}

void USelectionManager::OnUnitQueueChanged()
{
	OnUnitQueueUpdatedDelegate.Broadcast();
}

void USelectionManager::BindSelections()
{
	for(ABuilding* Building : SelectedBuildings)
	{
		Building->OnBuildingStateChangedDelegate.AddUniqueDynamic(this,&USelectionManager::OnBuildingStateChanged);
		Building->TeamEntity->OnTeamEntityKilledDelegate.AddUniqueDynamic(this,&USelectionManager::OnTeamEntityKilled);
		Building->TeamEntity->OnTeamEntityHPChanged.AddUniqueDynamic(this,&USelectionManager::OnTeamEntityHPChanged);

		if(USourceHolder* SourceHolder = Building->FindComponentByClass<USourceHolder>())
		{
			SourceHolder->OnSourceCollectedDelegate.AddUniqueDynamic(this,&USelectionManager::OnSourceCollected);
			SourceHolder->OnSourceFinishedDelegate.AddUniqueDynamic(this,&USelectionManager::OnSourceFinished);
		}

		if(UUnitGenerator* UnitGenerator = Building->FindComponentByClass<UUnitGenerator>())
		{
			UnitGenerator->OnProgressUpdated.AddUniqueDynamic(this,&USelectionManager::OnUnitGenerationProgressUpdated);
			UnitGenerator->OnUnitQueueChanged.AddUniqueDynamic(this,&USelectionManager::OnUnitQueueChanged);
		}

		Building->TeamEntity->SetSelected();
	}
	for(const AUnit* Unit : SelectedUnits)
	{
		Unit->TeamEntity->OnTeamEntityKilledDelegate.AddUniqueDynamic(this,&USelectionManager::OnTeamEntityKilled);
		Unit->TeamEntity->OnTeamEntityHPChanged.AddUniqueDynamic(this,&USelectionManager::OnTeamEntityHPChanged);

		Unit->TeamEntity->SetSelected();
	}
	for(const ASource* Source : SelectedSources)
	{
		Source->SourceHolder->OnSourceCollectedDelegate.AddUniqueDynamic(this,&USelectionManager::OnSourceCollected);
		Source->SourceHolder->OnSourceFinishedDelegate.AddUniqueDynamic(this,&USelectionManager::OnSourceFinished);

		Source->NeutralEntity->SetSelected();
	}
}
void USelectionManager::UnbindSelections()
{
	for(ABuilding* Building : SelectedBuildings)
	{
		Building->OnBuildingStateChangedDelegate.RemoveDynamic(this,&USelectionManager::OnBuildingStateChanged);
		Building->TeamEntity->OnTeamEntityKilledDelegate.RemoveDynamic(this,&USelectionManager::OnTeamEntityKilled);
		Building->TeamEntity->OnTeamEntityHPChanged.RemoveDynamic(this,&USelectionManager::OnTeamEntityHPChanged);

		if(USourceHolder* SourceHolder = Building->FindComponentByClass<USourceHolder>())
		{
			SourceHolder->OnSourceCollectedDelegate.RemoveDynamic(this,&USelectionManager::OnSourceCollected);
			SourceHolder->OnSourceFinishedDelegate.RemoveDynamic(this,&USelectionManager::OnSourceFinished);
		}

		if(UUnitGenerator* UnitGenerator = Building->FindComponentByClass<UUnitGenerator>())
		{
			UnitGenerator->OnProgressUpdated.RemoveDynamic(this,&USelectionManager::OnUnitGenerationProgressUpdated);
			UnitGenerator->OnUnitQueueChanged.RemoveDynamic(this,&USelectionManager::OnUnitQueueChanged);
		}

		Building->TeamEntity->SetUnSelected();
	}
	for(const AUnit* Unit : SelectedUnits)
	{
		Unit->TeamEntity->OnTeamEntityKilledDelegate.RemoveDynamic(this,&USelectionManager::OnTeamEntityKilled);
		Unit->TeamEntity->OnTeamEntityHPChanged.RemoveDynamic(this,&USelectionManager::OnTeamEntityHPChanged);

		Unit->TeamEntity->SetUnSelected();
	}
	for(const ASource* Source : SelectedSources)
	{
		Source->SourceHolder->OnSourceCollectedDelegate.RemoveDynamic(this,&USelectionManager::OnSourceCollected);
		Source->SourceHolder->OnSourceFinishedDelegate.RemoveDynamic(this,&USelectionManager::OnSourceFinished);

		Source->NeutralEntity->SetUnSelected();
	}
}
void USelectionManager::CreateSelectBox()
{
	//Preselect entity under cursor (if not selected already)
	if(const URTSEntity* Entity = PlayerManager->LookForRTSEntity())
	{
		UnbindSelections();
		SelectedSources.Empty();
		SelectedUnits.Empty();
		SelectedBuildings.Empty();
		if(ABuilding* Building = Cast<ABuilding>(Entity->GetOwner()))
		{
			if(!SelectedBuildings.Contains(Building))
			{
				SelectedBuildings.Add(Building);
			}
		}
		if(AUnit* Unit = Cast<AUnit>(Entity->GetOwner()))
		{
			if(!SelectedUnits.Contains(Unit))
			{
				SelectedUnits.Add(Unit);
			}
		}
		if(ASource* Source = Cast<ASource>(Entity->GetOwner()))
		{
			if(!SelectedSources.Contains(Source))
			{
				SelectedSources.Add(Source);
			}
		}

		BindSelections();
		UpdateSelectionState();
		
		return;
	}
	//---------------------------------------------------------------


	const FHitResult Hit = PlayerManager->LookForFloor();
	const FVector Location = Hit.Location;
	const FRotator Rotation(0);
	AActor* SpawnedActor = GameInstance->World->SpawnActor<AActor>(AssetManager->GetSelectBoxBP(),Location,Rotation);
	CurrentSelectBox = Cast<ASelectBox>(SpawnedActor);

	
	//Load overlaps to select box
	CurrentSelectBox->OverlappingBuildings = SelectedBuildings;
	CurrentSelectBox->OverlappingUnits = SelectedUnits;
	CurrentSelectBox->OverlappingSources = SelectedSources;
	
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
TArray<ASource*> USelectionManager::GetSelectedSources() const
{
	return SelectedSources;
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
	for(const ASource* Source : SelectedSources)
	{
		SelectedRTSEntities.Add(Source->NeutralEntity);
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
bool USelectionManager::AreSameSources(TArray<ASource*> Sources)
{
	if(Sources.Num() == 0)return true;

	const int SourceID = Sources[0]->NeutralEntity->GetEntityData().EntityID;
	for(const ASource* Source : Sources)
	{
		if(SourceID != Source->NeutralEntity->GetEntityData().EntityID)
		{
			return false;
		}
	}

	return true;
}
