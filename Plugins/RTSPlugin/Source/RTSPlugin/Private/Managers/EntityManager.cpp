// Fill out your copyright notice in the Description page of Project Settings.


#include "Managers/EntityManager.h"

#include "ActorsAndComponents/RTSEntity.h"
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
	Entity->OnEntityKilledDelegate.AddUniqueDynamic(this,&UEntityManager::OnEntityKilled);
}

void UEntityManager::OnEntityKilled(URTSEntity* Entity)
{
	Entities.Remove(Entity);
	Entity->OnEntityKilledDelegate.RemoveDynamic(this,&UEntityManager::OnEntityKilled);
	AActor* Owner = Entity->GetOwner();
	GameInstance->World->DestroyActor(Owner);
}
