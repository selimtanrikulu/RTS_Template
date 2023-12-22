// Fill out your copyright notice in the Description page of Project Settings.


#include "ActorsAndComponents/SelectBox.h"

#include "ActorsAndComponents/Building.h"
#include "ActorsAndComponents/Unit.h"
#include "Managers/PlayerManager.h"
#include "Managers/RTSGameInstance.h"

// Sets default values
ASelectBox::ASelectBox()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ASelectBox::BeginPlay()
{
	Super::BeginPlay();


	//Get dependencies
	const URTSGameInstance* GameInstance = Cast<URTSGameInstance>(GetGameInstance());
	PlayerManager = GameInstance->PlayerManager;
	//

	
	StartLocation = GetActorLocation();

}

// Called every frame
void ASelectBox::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);


	UpdateLocationAndScale();
	UpdateOverlaps();


}

void ASelectBox::UpdateLocationAndScale()
{
	const FHitResult Hit = PlayerManager->LookForFloor();
    const FVector HitLocation = Hit.Location;
    FVector DeltaLocation = HitLocation-StartLocation;
    
    FVector Scale(FMath::Abs(DeltaLocation.X)/100,FMath::Abs(DeltaLocation.Y)/100,0.1f);
    SetActorScale3D(Scale);

    FVector NewLocation = StartLocation + (DeltaLocation/2);
    SetActorLocation(NewLocation);
}

void ASelectBox::UpdateOverlaps()
{
	TArray<AActor*> OverlappingActors;
	GetOverlappingActors(OverlappingActors, UStaticMeshComponent::StaticClass());
	

	bool ChangedFlag = false;


	//Check new
	for(AActor* OverlappingActor : OverlappingActors)
	{
		if(ABuilding* Building = Cast<ABuilding>(OverlappingActor))
		{
			if(!OverlappingBuildings.Contains(Building))
			{
				OverlappingBuildings.Add(Building);
				ChangedFlag = true;
			}
		}

		if(AUnit* Unit = Cast<AUnit>(OverlappingActor))
		{
			if(!OverlappingUnits.Contains(Unit))
			{
				OverlappingUnits.Add(Unit);
				ChangedFlag = true;
			}
		}
	}


	//Check existence (Buildings)
	TArray<ABuilding*> BuildingsToRemove;
	for(ABuilding* OverlappingBuilding : OverlappingBuildings)
	{
		if(!OverlappingActors.Contains(OverlappingBuilding))
		{
			BuildingsToRemove.Add(OverlappingBuilding);
			ChangedFlag = true;
		}
	}
	for(ABuilding* BuildingToRemove : BuildingsToRemove)
	{
		OverlappingBuildings.Remove(BuildingToRemove);
	}

	//Check existence (Units)
	TArray<AUnit*> UnitsToRemove;
	for(AUnit* OverlappingUnit : OverlappingUnits)
	{
		if(!OverlappingActors.Contains(OverlappingUnit))
		{
			UnitsToRemove.Add(OverlappingUnit);
			ChangedFlag = true;
		}
	}
	for(AUnit* UnitToRemove : UnitsToRemove)
	{
		OverlappingUnits.Remove(UnitToRemove);
	}


	
	if(ChangedFlag)
	{
		OnOverlapChangedDelegate.Broadcast();
	}
}


