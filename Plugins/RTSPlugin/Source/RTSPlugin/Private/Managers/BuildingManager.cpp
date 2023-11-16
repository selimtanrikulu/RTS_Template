// Fill out your copyright notice in the Description page of Project Settings.


#include "Managers/BuildingManager.h"

#include "ActorsAndComponents/Building.h"
#include "ActorsAndComponents/TeamEntity.h"
#include "Kismet/GameplayStatics.h"
#include "Managers/RTSGameInstance.h"
#include "Managers/RTSPawn.h"

void UBuildingManager::Begin(UWorld* world)
{
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


void UBuildingManager::DraftBuilding(const FBuildingData& BuildingData)
{
	if(DraftingBuilding)
	{
		UE_LOG(LogTemp,Display,TEXT("Already drafting a building"));
		return;
	}
	
	DraftingBuilding = Cast<ABuilding>(World->SpawnActor(BuildingData.EntityData.BP));

	DraftingBuilding->SetBuildingState(EBuildingState::Draft);
	DraftingBuilding->BuildingData = BuildingData;
}

bool UBuildingManager::IsDrafting() const
{
	return DraftingBuilding != nullptr;
}

void UBuildingManager::UpdateDraftingBuildingLocation() const
{
	const FHitResult Hit = RTSPawn->LookForFloor();


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
	DraftingBuilding->SetBuildingState(EBuildingState::Located);
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
