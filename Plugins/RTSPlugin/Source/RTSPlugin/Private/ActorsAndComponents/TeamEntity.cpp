// Fill out your copyright notice in the Description page of Project Settings.


#include "ActorsAndComponents/TeamEntity.h"

#include "ActorsAndComponents/Building.h"
#include "ActorsAndComponents/Unit.h"

// Sets default values for this component's properties
UTeamEntity::UTeamEntity()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UTeamEntity::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UTeamEntity::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

FEntityData UTeamEntity::GetEntityData() const
{
	AActor* Owner = GetOwner();

	if(const ABuilding* Building = Cast<ABuilding>(Owner))
	{
		return Building->GetBuildingData().TeamEntityData.EntityData;
	}
	if(const AUnit* Unit = Cast<AUnit>(Owner))
	{
		return Unit->GetUnitData().TeamEntityData.EntityData;
	}


	UE_LOG(LogTemp,Error,TEXT("Entity Data cannot be found"));
	FEntityData GarbageEntity;
	GarbageEntity.Name = "Garbage";
	GarbageEntity.BP = nullptr;
	GarbageEntity.ImageMaterial = nullptr;
	GarbageEntity.EntityID = -1;
	return GarbageEntity;
}

FString UTeamEntity::GetInfo() const
{
	AActor* Owner = GetOwner();

	if(const ABuilding* Building = Cast<ABuilding>(Owner))
	{
		return Building->GetInfo();
	}
	if(const AUnit* Unit = Cast<AUnit>(Owner))
	{
		return Unit->GetInfo();
	}
	
	UE_LOG(LogTemp,Error,TEXT("Entity info not found"));
	return "Garbage Info";
}

float UTeamEntity::GetProgress() const
{
	return GetHPRatio();
}

void UTeamEntity::Init(const float currentHP)
{
	CurrentHP = currentHP;
}

void UTeamEntity::ApplyDamage(const float Damage)
{
	
	CurrentHP -= Damage;
	CurrentHP = FMath::Max(CurrentHP,0);


	
	OnTeamEntityGetDamageDelegate.Broadcast(this);
	
	if(CurrentHP <= 0)
	{
		Kill();
	}
}


float UTeamEntity::GetHPRatio() const
{
	float TotalHP;
	
	AActor* Owner = GetOwner();

	if(const ABuilding* Building = Cast<ABuilding>(Owner))
	{
		TotalHP = Building->GetBuildingData().TeamEntityData.HP;
	}
	else if(const AUnit* Unit = Cast<AUnit>(Owner))
	{
		TotalHP = Unit->GetUnitData().TeamEntityData.HP;
	}
	else
	{
		UE_LOG(LogTemp,Error,TEXT("HP ratio cannot be found"));
		return 0;
	}
	
	return CurrentHP / TotalHP;
}


