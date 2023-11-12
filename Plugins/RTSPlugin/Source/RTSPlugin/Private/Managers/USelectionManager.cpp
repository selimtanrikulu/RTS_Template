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
#include "ActorsAndComponents/Unit.h"

USelectionManager::USelectionManager()
{
}

USelectionManager::~USelectionManager()
{
}

void USelectionManager::Begin(UWorld* world,TSubclassOf<AActor> selectBoxBP)
{
	World = world;


	//Get dependencies
	const APlayerController* PlayerController = UGameplayStatics::GetPlayerController(World,0);
	const URTSGameInstance* GameInstance = Cast<URTSGameInstance>(PlayerController->GetGameInstance());
	
	LogManager = GameInstance->LogManager;
	//
	
	RTSPawn = Cast<ARTSPawn>(Util::GetActorOfClass(world,ARTSPawn::StaticClass()));


	//Bind Inputs
	RTSPawn->OnMouseLeftClickDelegate.AddUniqueDynamic(this,&USelectionManager::OnMouseLeftClicked);
	RTSPawn->OnMouseLeftReleasedDelegate.AddUniqueDynamic(this,&USelectionManager::OnMouseLeftReleased);

	
	SelectBoxBP = selectBoxBP;
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

void USelectionManager::OnOverlapChanged()
{
	//Update selection
	SelectedBuildings = CurrentSelectBox->OverlappingBuildings;
	SelectedUnits = CurrentSelectBox->OverlappingUnits;

	//Fire main delegate
	OnSelectionChangedDelegate.Broadcast();
}

void USelectionManager::UpdateCircles()
{
	for(const ABuilding* SelectedBuilding : SelectedBuildings)
	{
		DrawCircle(World,
		SelectedBuilding->MeshComponent->GetComponentLocation(),
		FVector::RightVector,
		FVector::ForwardVector,
		FColor::Blue,
		SelectedBuilding->Extent,
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
		SelectedUnit->MeshComponent->GetComponentLocation(),
		FVector::RightVector,
		FVector::ForwardVector,
		FColor::Yellow,
		SelectedUnit->Extent,
		300,
		false,
		-1,
		0,
		20
		);
	}

}

void USelectionManager::CreateSelectBox()
{
	const FHitResult Hit = RTSPawn->LookForFloor();

	const FVector Location = Hit.Location;
	const FRotator Rotation(0);
	
	AActor* SpawnedActor = World->SpawnActor<AActor>(SelectBoxBP,Location,Rotation);
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