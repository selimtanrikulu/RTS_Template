// Fill out your copyright notice in the Description page of Project Settings.


#include "ActorsAndComponents/Worker.h"

#include "ActorsAndComponents/RTSEntity.h"
#include "ActorsAndComponents/SourceHolder.h"

void AWorker::BeginPlay()
{
	Super::BeginPlay();
}

void AWorker::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);


	if(CollectingSource)
	{
		if(IsCollectingSourceInRange())
		{
			CollectingSource->Collect();
		}
	}
}

void AWorker::SetTargetRTSEntity(const URTSEntity* Entity)
{
	Super::SetTargetRTSEntity(Entity);
	
	if(USourceHolder* SourceHolder = Entity->GetOwner()->FindComponentByClass<USourceHolder>())
	{
		UnbindSource();
		BindSource(SourceHolder);
	}
}

void AWorker::SetTargetLocation(const FVector& TargetLocation)
{
	Super::SetTargetLocation(TargetLocation);

	UnbindSource();
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


bool AWorker::IsCollectingSourceInRange() const
{
	if(!CollectingSource)
	{
		UE_LOG(LogTemp,Error,TEXT("No collecting source to be in range"));
		return false;
	}

	const FVector WorkerLocation = GetActorLocation();
	const FVector SourceLocation = CollectingSource->GetOwner()->GetActorLocation();
	const float Dist = (SourceLocation-WorkerLocation).Length();
	const URTSEntity* Entity = CollectingSource->GetOwner()->FindComponentByClass<URTSEntity>();

	if(!Entity)
	{
		UE_LOG(LogTemp,Error,TEXT("Collecting source has no entity"))
		return false; 
	}

	const float Extent = Entity->Extent;
	return Dist <= Extent+CollectRange;
}
