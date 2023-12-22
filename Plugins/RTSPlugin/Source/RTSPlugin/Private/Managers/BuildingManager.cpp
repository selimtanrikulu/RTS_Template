// Fill out your copyright notice in the Description page of Project Settings.


#include "Managers/BuildingManager.h"

#include "PlayerManager.h"
#include "ActorsAndComponents/Building.h"
#include "Managers/RTSGameInstance.h"


void UBuildingManager::Init(URTSGameInstance* gameInstance)
{
	Super::Init(gameInstance);

	PlayerManager = GameInstance->PlayerManager;
}

void UBuildingManager::Begin()
{
	Super::Begin();

	PlayerManager->OnMouseLeftClickDelegate.AddUniqueDynamic(this,&UBuildingManager::OnMouseLeftClicked);
	PlayerManager->OnMouseRightClickDelegate.AddUniqueDynamic(this,&UBuildingManager::OnMouseRightClicked);
	PlayerManager->OnMouseWheelUpDelegate.AddUniqueDynamic(this,&UBuildingManager::OnMouseWheelUp);
	PlayerManager->OnMouseWheelDownDelegate.AddUniqueDynamic(this,&UBuildingManager::OnMouseWheelDown);
}

void UBuildingManager::Tick(float DeltaTime)
{
	if(DraftingBuilding)
	{
		UpdateDraftingBuildingLocation();
	}
}

void UBuildingManager::OnMouseLeftClicked()
{
	if(DraftingBuilding)
	{
		LocateBuilding();
	}
}

void UBuildingManager::OnMouseRightClicked()
{
	if(DraftingBuilding)
	{
		CancelDraft();
	}
}

void UBuildingManager::OnMouseWheelUp()
{
	if(DraftingBuilding)
	{
		RotateBuilding(15);
	}
}

void UBuildingManager::OnMouseWheelDown()
{
	if(DraftingBuilding)
	{
		RotateBuilding(-15);
	}
}


void UBuildingManager::DraftBuilding(const FBuildingData& BuildingData)
{
	if(DraftingBuilding)
	{
		UE_LOG(LogTemp,Display,TEXT("Already drafting a building"));
		return;
	}
	
	DraftingBuilding = Cast<ABuilding>(GameInstance->World->SpawnActor(BuildingData.EntityData.BP));

	DraftingBuilding->BuildingData = BuildingData;
	DraftingBuilding->CacheMaterials();
	DraftingBuilding->SetBuildingState(EBuildingState::Draft);
}

bool UBuildingManager::IsDrafting() const
{
	return DraftingBuilding != nullptr;
}

void UBuildingManager::UpdateDraftingBuildingLocation() const
{
	const FHitResult Hit = PlayerManager->LookForFloor();


	const EBuildingState BuildingState = DraftingBuilding->GetBuildingState();
	const bool Locatable = DraftingBuilding->Locatable();
	
	
	if(Locatable && BuildingState != EBuildingState::Draft)
	{
		DraftingBuilding->SetBuildingState(EBuildingState::Draft);
	}
	else if(!Locatable && BuildingState != EBuildingState::Error)
	{
		DraftingBuilding->SetBuildingState(EBuildingState::Error);
	}

	
	DraftingBuilding->SetActorLocation(Hit.Location);
}
void UBuildingManager::LocateBuilding()
{
	DraftingBuilding->SetBuildingState(EBuildingState::Construction);
	DraftingBuilding = nullptr;
}
void UBuildingManager::CancelDraft()
{
	GameInstance->World->DestroyActor(DraftingBuilding);
	DraftingBuilding = nullptr;
}
void UBuildingManager::RotateBuilding(const float Amount) const
{
	DraftingBuilding->AddActorWorldRotation(FRotator(0,Amount,0));
}
