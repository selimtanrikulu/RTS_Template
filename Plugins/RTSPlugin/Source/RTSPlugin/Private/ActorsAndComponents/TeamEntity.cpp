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

void UTeamEntity::Init(const FTeamEntityData &teamEntityData)
{
	TeamEntityData = teamEntityData;
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
	return TeamEntityData.EntityData;
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



void UTeamEntity::ApplyDamage(const float Damage)
{
	CurrentHP -= Damage;
	CurrentHP = FMath::Max(CurrentHP,0);
	
	OnTeamEntityHPChanged.Broadcast(this);
	
	if(CurrentHP <= 0)
	{
		OnEntityKilledDelegate.Broadcast(this);
	}
}

void UTeamEntity::Heal(float Amount)
{
	CurrentHP += Amount;
	CurrentHP = FMath::Min(CurrentHP,TeamEntityData.HP);
	
	OnTeamEntityHPChanged.Broadcast(this);
}

void UTeamEntity::HealFull()
{
    CurrentHP = TeamEntityData.HP;
    OnTeamEntityHPChanged.Broadcast(this);
}

float UTeamEntity::GetCurrentHP() const
{
	return CurrentHP;
}


float UTeamEntity::GetHPRatio() const
{
	return CurrentHP / TeamEntityData.HP;
}


