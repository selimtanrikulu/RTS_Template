// Fill out your copyright notice in the Description page of Project Settings.


#include "ActorsAndComponents/Source.h"

#include "ActorsAndComponents/SourceHolder.h"

// Sets default values
ASource::ASource()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	SourceHolder = CreateDefaultSubobject<USourceHolder>(TEXT("SourceHolder"));
	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>("StaticMeshComponent");
	SetRootComponent(StaticMeshComponent);
}

void ASource::Init(const FSourceData& SourceData) const
{
	SourceHolder->Init(SourceData);
}

// Called when the game starts or when spawned
void ASource::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ASource::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

