// Fill out your copyright notice in the Description page of Project Settings.


#include "Managers/BuildingManager.h"

#include "ActorsAndComponents/Building.h"
#include "Kismet/GameplayStatics.h"
#include "Managers/RTSGameInstance.h"
#include "Managers/RTSPawn.h"

void UBuildingManager::Begin(UWorld* world, const FBuildingManagerConfig& buildingManagerConfig)
{
	BuildingManagerConfig = buildingManagerConfig;
	World = world;


	const APlayerController* PlayerController = UGameplayStatics::GetPlayerController(World,0);
	URTSGameInstance* GameInstance = Cast<URTSGameInstance>(PlayerController->GetGameInstance());

	RTSPawn = Cast<ARTSPawn>(Util::GetActorOfClass(World,ARTSPawn::StaticClass()));

	
	RTSPawn->OnMouseLeftClickDelegate.AddUniqueDynamic(this,&UBuildingManager::OnMouseLeftClicked);
	RTSPawn->OnMouseRightClickDelegate.AddUniqueDynamic(this,&UBuildingManager::OnMouseRightClicked);
	RTSPawn->OnMouseWheelUpDelegate.AddUniqueDynamic(this,&UBuildingManager::OnMouseWheelUp);
	RTSPawn->OnMouseWheelDownDelegate.AddUniqueDynamic(this,&UBuildingManager::OnMouseWheelDown);
	
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


TArray<FBuildingData> UBuildingManager::GetBuildingsData() const
{
	return BuildingManagerConfig.BuildingsData;
}
void UBuildingManager::DraftBuilding(const FBuildingData& BuildingData)
{
	DraftingBuilding = Cast<ABuilding>(World->SpawnActor(BuildingData.BP));
	
}
void UBuildingManager::UpdateDraftingBuildingLocation() const
{
	const FHitResult Hit = RTSPawn->LookForFloor();
	
	DraftingBuilding->SetActorLocation(Hit.Location);
}
void UBuildingManager::LocateBuilding()
{
	DraftingBuilding = nullptr;
}
void UBuildingManager::CancelDraft()
{
	World->DestroyActor(DraftingBuilding);
	DraftingBuilding = nullptr;
}
void UBuildingManager::RotateBuilding(const float Amount) const
{
	DraftingBuilding->AddActorWorldRotation(FRotator(0,Amount,0));
}
