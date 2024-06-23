// Fill out your copyright notice in the Description page of Project Settings.


#include "ActorsAndComponents/UnitGenerator.h"

#include "ActorsAndComponents/Building.h"
#include "ActorsAndComponents/TeamEntity.h"
#include "ActorsAndComponents/Unit.h"
#include "Kismet/GameplayStatics.h"
#include "Managers/EntityManager.h"
#include "Managers/RTSGameInstance.h"
#include "Managers/SourceManager.h"
#include "Managers/StoreManager.h"

// Sets default values for this component's properties
UUnitGenerator::UUnitGenerator()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UUnitGenerator::BeginPlay()
{
	Super::BeginPlay();

	// ...

	const APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(),0);
	GameInstance = Cast<URTSGameInstance>(PlayerController->GetGameInstance());


	StoreManager = GameInstance->StoreManager;
	EntityManager = GameInstance->EntityManager;
	SourceManager = GameInstance->SourceManager;

	OwnerBuilding = Cast<ABuilding>(GetOwner());
}


// Called every frame
void UUnitGenerator::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...

	Delta_Time = DeltaTime;

	if(UnitQueue.Num() > 0)
	{
		ProgressGeneration(DeltaTime);
	} 
}

TArray<FUnitData>& UUnitGenerator::GetUnits()
{
	if(Units.Num() == 0)
	{
		//Lazy load (escape begin play)
		for(FString UnitName : UnitNames)
		{
			const FUnitData* UnitData = StoreManager->GetUnitByName(UnitName);

			if(UnitData)
			{
				Units.Add(*UnitData);
			}
		}
	}
	
	return Units;
}

void UUnitGenerator::AddUnitToQueue(const FUnitData& UnitData)
{
	if(!SourceManager->CheckBalance(UnitData.TeamEntityData))
	{
		//Not enough money
		return;
	}
	
	if(UnitQueue.Num() < GameInstance->MaxUnitGenerationQueue)
	{
		SourceManager->Buy(UnitData.TeamEntityData);
		UnitQueue.Add(UnitData);

		OnUnitQueueChanged.Broadcast();
	}
	else
	{
		//Queue is max
	}
}

void UUnitGenerator::PopUnitFromQueue(int Index)
{
	if(UnitQueue.Num() > Index && Index >= 0)
	{
		//TODO : add cost back to sources
		UnitQueue.RemoveAt(Index);

		GenerationTime = 0;
		OnProgressUpdated.Broadcast();
		OnUnitQueueChanged.Broadcast();
	}
	else
	{
		UE_LOG(LogTemp,Error,TEXT("Trying to pop index out of boundary"));
	}
}

float UUnitGenerator::GetProgress() const
{
	if(UnitQueue.Num() > 0)
	{
		return GenerationTime / UnitQueue[0].GenerationTime;
	}

	return 0;
}

TArray<FUnitData> UUnitGenerator::GetUnitQueue() const
{
	return UnitQueue;
}


void UUnitGenerator::ProgressGeneration(float Batch)
{
	GenerationTime += Batch;

	if(GetProgress() >= 1)
	{
		GenerationTime = 0;
		SpawnUnit();
		PopUnitFromQueue(0);
		OnUnitQueueChanged.Broadcast();
	}
	
	OnProgressUpdated.Broadcast();
}

// Function to check if a location is occupied
bool UUnitGenerator::IsLocationOccupied(const FVector& Location, float Radius) const
{
    FHitResult HitResult;
    FCollisionQueryParams CollisionParams;
	
	
    return GetWorld()->SweepSingleByChannel
		(HitResult,
		Location,
		Location,
		FQuat::Identity,
		ECC_WorldDynamic,
		FCollisionShape::MakeSphere(Radius),
		CollisionParams);
}


// Function to find the closest empty location near a building
FVector UUnitGenerator::FindClosestEmptyLocation(const float UnitExtent) const
{
	const FVector BuildingLocation = GetOwner()->GetActorLocation();
	

	const float BuildingExtent = OwnerBuilding->TeamEntity->Extent;
	constexpr float Bias = 10;

	const float SearchRadius = BuildingExtent + UnitExtent + Bias;

	const float MaxRadius = SearchRadius + 1000;

	float CurrentSearchRadius = SearchRadius;

	
	while(CurrentSearchRadius <= MaxRadius)
	{
		for (float Angle = 0.0f; Angle < 2.0f * PI; Angle += 0.1f) // Adjust the angle increment as needed
		{
			FVector SamplePoint = BuildingLocation +
			FVector(CurrentSearchRadius * FMath::Cos(Angle),
				CurrentSearchRadius * FMath::Sin(Angle),
				UnitExtent+Bias);

			// Check if the sample point is occupied
			if (!IsLocationOccupied(SamplePoint, UnitExtent)) // Adjust the radius as needed
			{
				const FVector ClosestPoint(SamplePoint.X,SamplePoint.Y,0);
				return ClosestPoint; // Found an empty location, return it
			}
		}
		
		CurrentSearchRadius += UnitExtent + Bias;
	}
	

	return FVector(-1000,-1000,-1000);
}



// Function to spawn a unit at the closest empty location
void UUnitGenerator::SpawnUnit() const
{
	if(UnitQueue.Num() == 0)
	{
		UE_LOG(LogTemp,Error,TEXT("Cannot spawn unit, queue is empty"));
		return;
	}

	const FUnitData UnitData = UnitQueue[0];
	
	constexpr float UnitExtent = 100;
	const TSubclassOf<AActor> UnitBP = UnitData.TeamEntityData.EntityData.BP;
	const FVector SpawnLocation = FindClosestEmptyLocation(UnitExtent);

	if(SpawnLocation.Z < -100)
	{
		UE_LOG(LogTemp,Warning,TEXT("No location found for unit"));
		return;
	}
	
	
	AActor* SpawnedActor = GetWorld()->SpawnActor<AActor>(
	UnitBP,
	SpawnLocation,
	FRotator::ZeroRotator);

	
	AUnit* SpawnedUnit = Cast<AUnit>(SpawnedActor);
	if (SpawnedUnit)
	{
		SpawnedUnit->Init(UnitData);
	}
	EntityManager->AddEntity(SpawnedUnit->TeamEntity);
	
}
