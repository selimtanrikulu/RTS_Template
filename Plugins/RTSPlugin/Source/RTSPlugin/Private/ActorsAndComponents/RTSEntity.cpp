// Fill out your copyright notice in the Description page of Project Settings.


#include "ActorsAndComponents/RTSEntity.h"
#include "Managers/RTSGameInstance.h"

// Sets default values for this component's properties
URTSEntity::URTSEntity()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	
	// ...
}


// Called when the game starts
void URTSEntity::BeginPlay()
{
	Super::BeginPlay();

	// ...

	MeshComponent = GetOwner()->FindComponentByClass<UMeshComponent>();
	if(MeshComponent)
	{
		BoxExtent = MeshComponent->GetLocalBounds().BoxExtent;
		Extent = FMath::Sqrt(BoxExtent.X*BoxExtent.X + BoxExtent.Y*BoxExtent.Y);
	}
}


// Called every frame
void URTSEntity::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

FEntityData URTSEntity::GetEntityData() const
{
	UE_LOG(LogTemp,Error,TEXT("Entity Data cannot be found"));
	FEntityData GarbageEntity;
	GarbageEntity.Name = "Garbage";
	GarbageEntity.BP = nullptr;
	GarbageEntity.ImageMaterial = nullptr;
	GarbageEntity.EntityID = -1;
	return GarbageEntity;
}

FString URTSEntity::GetInfo() const
{
	UE_LOG(LogTemp,Error,TEXT("Entity info not found"));
	return "Garbage Info";
}

float URTSEntity::GetProgress() const
{
	UE_LOG(LogTemp,Error,TEXT("Entity progress not found"));
	return 0;
}

