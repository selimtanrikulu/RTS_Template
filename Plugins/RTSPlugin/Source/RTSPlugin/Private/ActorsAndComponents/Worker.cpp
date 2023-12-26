// Fill out your copyright notice in the Description page of Project Settings.


#include "ActorsAndComponents/Worker.h"
#include "ActorsAndComponents/Building.h"
#include "ActorsAndComponents/RTSEntity.h"
#include "ActorsAndComponents/SourceHolder.h"

void AWorker::BeginPlay()
{
	Super::BeginPlay();
}

void AWorker::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);


	if(IsCollectingSource())
	{
		CollectingSource->Collect();
	}
	

	if(IsConstructingBuilding())
	{
		ConstructingBuilding->Progress(DeltaSeconds*30);
	}
}

void AWorker::SetTargetRTSEntity(const URTSEntity* Entity)
{
	Super::SetTargetRTSEntity(Entity);
	
	if(USourceHolder* SourceHolder = Entity->GetOwner()->FindComponentByClass<USourceHolder>())
	{
		UnbindSource();
		UnbindBuilding();
		BindSource(SourceHolder);
	}

	if(ABuilding* Building = Cast<ABuilding>(Entity->GetOwner()))
	{
		if(Building->GetBuildingState() == EBuildingState::Construction)
		{
			UnbindSource();
			UnbindBuilding();
			BindBuilding(Building);
		}
	}
}

void AWorker::SetTargetLocation(const FVector& TargetLocation)
{
	Super::SetTargetLocation(TargetLocation);

	UnbindSource();
	UnbindBuilding();
}

bool AWorker::IsCollectingSource() const
{
	return CollectingSource && IsInRange(CollectingSource->GetOwner()->FindComponentByClass<URTSEntity>());
}

bool AWorker::IsConstructingBuilding() const
{
	return ConstructingBuilding && IsInRange(ConstructingBuilding->TeamEntity);
}


void AWorker::UpdateUnitAnimation()
{
	Super::UpdateUnitAnimation();
	
	if(IsCollectingSource())
	{
		UnitAnimationState = EUnitAnimationState::Collect;
	}
	else if(IsConstructingBuilding())
	{
		UnitAnimationState = EUnitAnimationState::Construct;
	}
}

void AWorker::OnCollectingSourceFinished(USourceHolder* SourceHolder)
{
	if(SourceHolder != CollectingSource)
	{
		UE_LOG(LogTemp,Error,TEXT("Bound source is not collecting source"));
		return;
	}
	
	CollectingSource = nullptr;
}

void AWorker::BindSource(USourceHolder* SourceHolder)
{
	if(CollectingSource)
	{
		UE_LOG(LogTemp,Error,TEXT("Already bound a source"));
		return;
	}
	
	CollectingSource = SourceHolder;
	CollectingSource->OnSourceFinishedDelegate.AddUniqueDynamic(this,&AWorker::OnCollectingSourceFinished);
}

void AWorker::UnbindSource()
{
	if(CollectingSource)
	{
		CollectingSource->OnSourceFinishedDelegate.RemoveDynamic(this,&AWorker::OnCollectingSourceFinished);
		CollectingSource = nullptr;
	}
}

void AWorker::OnBuildingChanged(UTeamEntity* teamEntity)
{
	if(!ConstructingBuilding)
	{
		UE_LOG(LogTemp,Error,TEXT("Not bound to a building"));
		return;
	}

	if(ConstructingBuilding->TeamEntity != teamEntity)
	{
		UE_LOG(LogTemp,Error,TEXT("Changed building is not constructing building"));
		return;
	}

	const EBuildingState BuildingState = ConstructingBuilding->GetBuildingState();
	if(BuildingState == EBuildingState::Located)
	{
		//Construction finished
		UnbindBuilding();
	}
}

void AWorker::OnBuildingKilled(UTeamEntity* teamEntity)
{
	if(!ConstructingBuilding)
	{
		UE_LOG(LogTemp,Error,TEXT("Not bound to a building"));
		return;
	}

	if(ConstructingBuilding->TeamEntity != teamEntity)
	{
		UE_LOG(LogTemp,Error,TEXT("Changed building is not constructing building"));
		return;
	}

	UnbindBuilding();
}

void AWorker::BindBuilding(ABuilding* Building)
{
	if(ConstructingBuilding)
	{
		UE_LOG(LogTemp,Error,TEXT("Already bound a building"));
		return;
	}
	
	ConstructingBuilding = Building;
	ConstructingBuilding->OnBuildingChangedDelegate.AddUniqueDynamic(this,&AWorker::OnBuildingChanged);
	ConstructingBuilding->TeamEntity->OnTeamEntityKilledDelegate.AddUniqueDynamic(this,&AWorker::OnBuildingKilled);
}

void AWorker::UnbindBuilding()
{
	if(ConstructingBuilding)
	{
		ConstructingBuilding->OnBuildingChangedDelegate.RemoveDynamic(this,&AWorker::OnBuildingChanged);
		ConstructingBuilding->TeamEntity->OnTeamEntityKilledDelegate.RemoveDynamic(this,&AWorker::OnBuildingKilled);
		ConstructingBuilding = nullptr;
	}
}


bool AWorker::IsInRange(const URTSEntity* Entity) const
{
	const FVector WorkerLocation = GetActorLocation();
	const FVector EntityLocation = Entity->GetOwner()->GetActorLocation();
	const float Dist = (EntityLocation-WorkerLocation).Length();
	const float Extent = Entity->Extent;
	return Dist <= Extent+WorkerRange;
}
