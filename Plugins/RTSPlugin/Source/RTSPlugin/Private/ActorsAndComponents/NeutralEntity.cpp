// Fill out your copyright notice in the Description page of Project Settings.


#include "ActorsAndComponents/NeutralEntity.h"

#include "ActorsAndComponents/SourceHolder.h"

UNeutralEntity::UNeutralEntity()
{
}

void UNeutralEntity::BeginPlay()
{
	Super::BeginPlay();
}

void UNeutralEntity::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

FEntityData UNeutralEntity::GetEntityData() const
{
	const AActor* Owner = GetOwner();

	if(const USourceHolder* SourceHolder = Owner->FindComponentByClass<USourceHolder>())
	{
		return SourceHolder->GetSourceData().EntityData;
	}

	UE_LOG(LogTemp,Error,TEXT("Entity Data cannot be found"));
	FEntityData GarbageEntity;
	GarbageEntity.Name = "Garbage";
	GarbageEntity.BP = nullptr;
	GarbageEntity.ImageMaterial = nullptr;
	GarbageEntity.EntityID = -1;
	return GarbageEntity;
}

FString UNeutralEntity::GetInfo() const
{
	const AActor* Owner = GetOwner();
	
	if(const USourceHolder* SourceHolder = Owner->FindComponentByClass<USourceHolder>())
	{
		return SourceHolder->GetInfo();
	}
	
	UE_LOG(LogTemp,Error,TEXT("Entity info not found"));
	return "Garbage Info";
}

float UNeutralEntity::GetProgress() const
{
	const AActor* Owner = GetOwner();

	if(const USourceHolder* SourceHolder = Owner->FindComponentByClass<USourceHolder>())
	{
		return SourceHolder->GetProgress();
	}

	UE_LOG(LogTemp,Error,TEXT("Progress not found"));
	return 0;
}
