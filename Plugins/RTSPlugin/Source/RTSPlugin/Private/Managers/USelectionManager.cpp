// Fill out your copyright notice in the Description page of Project Settings.


#include "Managers/USelectionManager.h"

#include "ActorsAndComponents/SelectBox.h"
#include "Kismet/GameplayStatics.h"
#include "Managers/LogManager.h"
#include "Managers/RTSGameInstance.h"
#include "Managers/RTSPawn.h"
#include "Utility/Util.h"
#include "SceneManagement.h"
#include "ActorsAndComponents/Building.h"
#include "ActorsAndComponents/TeamEntity.h"
#include "ActorsAndComponents/Unit.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Managers/Asset_Manager.h"

USelectionManager::USelectionManager()
{
}

USelectionManager::~USelectionManager()
{
}

void USelectionManager::Begin(UWorld* world)
{
	World = world;


	//Get dependencies
	const APlayerController* PlayerController = UGameplayStatics::GetPlayerController(World,0);
	const URTSGameInstance* GameInstance = Cast<URTSGameInstance>(PlayerController->GetGameInstance());
	
	LogManager = GameInstance->LogManager;
	AssetManager = GameInstance->AssetManager;
	//
	
	RTSPawn = Cast<ARTSPawn>(Util::GetActorOfClass(world,ARTSPawn::StaticClass()));


	//Bind Inputs
	RTSPawn->OnMouseLeftClickDelegate.AddUniqueDynamic(this,&USelectionManager::OnMouseLeftClicked);
	RTSPawn->OnMouseLeftReleasedDelegate.AddUniqueDynamic(this,&USelectionManager::OnMouseLeftReleased);
	RTSPawn->OnMouseRightClickDelegate.AddUniqueDynamic(this,&USelectionManager::OnMouseRightClicked);

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
	const FHitResult Hit = RTSPawn->LookForFloor();
	
	for(AUnit* SelectedUnit : SelectedUnits)
	{
		SelectedUnit->SetTargetLocation(Hit.Location);
	}
}

void USelectionManager::OnOverlapChanged()
{
	//Update selection
	SelectedBuildings = CurrentSelectBox->OverlappingBuildings;
	SelectedUnits = CurrentSelectBox->OverlappingUnits;

	UpdateSelectionState();
	
	//Fire main delegate
	OnSelectionChangedDelegate.Broadcast();
}

void USelectionManager::UpdateCircles()
{
	for(const ABuilding* SelectedBuilding : SelectedBuildings)
	{
		DrawCircle(World,
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
		DrawCircle(World,
		SelectedUnit->TeamEntity->MeshComponent->GetComponentLocation(),
		FVector::RightVector,
		FVector::ForwardVector,
		FColor::White,
		SelectedUnit->TeamEntity->Extent,
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

void USelectionManager::CreateSelectBox()
{
	const FHitResult Hit = RTSPawn->LookForFloor();

	const FVector Location = Hit.Location;
	const FRotator Rotation(0);
	
	AActor* SpawnedActor = World->SpawnActor<AActor>(AssetManager->GetSelectBoxBP(),Location,Rotation);
	CurrentSelectBox = Cast<ASelectBox>(SpawnedActor);
	
	//Load overlaps to select box
	CurrentSelectBox->OverlappingBuildings = SelectedBuildings;
	CurrentSelectBox->OverlappingUnits = SelectedUnits;
	
	CurrentSelectBox->OnOverlapChangedDelegate.AddUniqueDynamic(this,&USelectionManager::OnOverlapChanged);
}

void USelectionManager::DestroySelectBox()
{
	CurrentSelectBox->OnOverlapChangedDelegate.RemoveDynamic(this,&USelectionManager::OnOverlapChanged);
	World->DestroyActor(CurrentSelectBox);
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

ESelectionState USelectionManager::GetSelectionState() const
{
	return SelectionState;
}


bool USelectionManager::AreSameBuildings(TArray<ABuilding*> Buildings)
{
	if(Buildings.Num() == 0)return true;

	const int BuildingID = Buildings[0]->BuildingData.EntityData.EntityID;
	for(const ABuilding* Building : Buildings)
	{
		if(BuildingID != Building->BuildingData.EntityData.EntityID)
		{
			return false;
		}
	}

	return true;
}

bool USelectionManager::AreSameUnits(TArray<AUnit*> Units)
{
	if(Units.Num() == 0)return true;

	const int UnitID = Units[0]->UnitData.EntityData.EntityID;
	for(const AUnit* Unit : Units)
	{
		if(UnitID != Unit->UnitData.EntityData.EntityID)
		{
			return false;
		}
	}

	return true;
}
