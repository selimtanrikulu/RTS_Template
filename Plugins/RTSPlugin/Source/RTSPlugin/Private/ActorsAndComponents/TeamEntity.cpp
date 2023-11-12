// Fill out your copyright notice in the Description page of Project Settings.


#include "ActorsAndComponents/TeamEntity.h"

// Sets default values
ATeamEntity::ATeamEntity()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	
}

// Called when the game starts or when spawned
void ATeamEntity::BeginPlay()
{
	Super::BeginPlay();
	
	MeshComponent = FindComponentByClass<UMeshComponent>();


	if(MeshComponent)
	{
		BoxExtent = MeshComponent->GetLocalBounds().BoxExtent;
		Extent = FMath::Sqrt(BoxExtent.X*BoxExtent.X + BoxExtent.Y*BoxExtent.Y);
	}
}

// Called every frame
void ATeamEntity::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

