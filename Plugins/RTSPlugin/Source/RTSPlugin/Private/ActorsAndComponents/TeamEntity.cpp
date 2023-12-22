// Fill out your copyright notice in the Description page of Project Settings.


#include "ActorsAndComponents/TeamEntity.h"

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


	

	
	MeshComponent = GetOwner()->FindComponentByClass<UMeshComponent>();
	if(MeshComponent)
	{
		BoxExtent = MeshComponent->GetLocalBounds().BoxExtent;
		Extent = FMath::Sqrt(BoxExtent.X*BoxExtent.X + BoxExtent.Y*BoxExtent.Y);
	}


	
}


// Called every frame
void UTeamEntity::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}


