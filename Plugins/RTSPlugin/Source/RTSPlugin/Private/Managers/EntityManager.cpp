// Fill out your copyright notice in the Description page of Project Settings.


#include "Managers/EntityManager.h"
#include "ActorsAndComponents/RTSEntity.h"
#include "ActorsAndComponents/SourceHolder.h"
#include "ActorsAndComponents/TeamEntity.h"
#include "Managers/RTSGameInstance.h"

void UEntityManager::Init(URTSGameInstance* gameInstance)
{
	Super::Init(gameInstance);
}

void UEntityManager::Begin()
{
	Super::Begin();
}

void UEntityManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void UEntityManager::AddEntity(URTSEntity* Entity)
{
	Entities.Add(Entity);

	if(UTeamEntity* TeamEntity = Cast<UTeamEntity>(Entity))
	{
		TeamEntity->OnTeamEntityKilledDelegate.AddUniqueDynamic(this,&UEntityManager::OnEntityKilled);
	}

	if(USourceHolder* SourceHolder = Entity->GetOwner()->FindComponentByClass<USourceHolder>())
	{
		SourceHolder->OnSourceFinishedDelegate.AddUniqueDynamic(this,&UEntityManager::OnSourceFinished);
	}
}

void UEntityManager::OnEntityKilled(UTeamEntity* TeamEntity)
{
	Entities.Remove(TeamEntity);
	TeamEntity->OnTeamEntityKilledDelegate.RemoveDynamic(this,&UEntityManager::OnEntityKilled);
	AActor* Owner = TeamEntity->GetOwner();
	GameInstance->World->DestroyActor(Owner);
}

void UEntityManager::OnSourceFinished(USourceHolder* SourceHolder)
{
	AActor* Owner = SourceHolder->GetOwner();
	URTSEntity* Entity = Owner->FindComponentByClass<URTSEntity>();
	Entities.Remove(Entity);
	SourceHolder->OnSourceFinishedDelegate.RemoveDynamic(this,&UEntityManager::OnSourceFinished);
	GameInstance->World->DestroyActor(Owner);
}
