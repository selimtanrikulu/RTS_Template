// Fill out your copyright notice in the Description page of Project Settings.


#include "ActorsAndComponents/SelectBox.h"

#include "Managers/RTSPawn.h"
#include "Utility/Util.h"

// Sets default values
ASelectBox::ASelectBox()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ASelectBox::BeginPlay()
{
	Super::BeginPlay();


	//Get dependencies
	RTSPawn = Cast<ARTSPawn>(Util::GetActorOfClass(GetWorld(),ARTSPawn::StaticClass()));
	//

	
	StartLocation = GetActorLocation();
}

// Called every frame
void ASelectBox::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);


	UpdateLocationAndScale();
}

void ASelectBox::UpdateLocationAndScale()
{
	const FHitResult Hit = RTSPawn->LookForFloor();
    const FVector HitLocation = Hit.Location;
    FVector DeltaLocation = HitLocation-StartLocation;
    
    FVector Scale(FMath::Abs(DeltaLocation.X)/100,FMath::Abs(DeltaLocation.Y)/100,0.1f);
    SetActorScale3D(Scale);

    FVector NewLocation = StartLocation + (DeltaLocation/2);
    SetActorLocation(NewLocation);
}

