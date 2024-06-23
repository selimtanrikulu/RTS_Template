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

	
	
	SelectedMesh = CreateDefaultSubobject<UStaticMeshComponent>("Selected");
	
	SelectedMesh->SetCollisionProfileName("NoCollision");
	SelectedMesh->SetVisibility(false);


    static ConstructorHelpers::FObjectFinder<UStaticMesh> MeshAsset(TEXT("/RTSPlugin/Shapes/Torus.Torus"));

    // Check if the mesh asset is found
    if (MeshAsset.Succeeded())
    {
        // Assign the static mesh to the static mesh component
        SelectedMesh->SetStaticMesh(MeshAsset.Object);
    }

	static ConstructorHelpers::FObjectFinder<UMaterial> MaterialAsset(TEXT("/RTSPlugin/Materials/M_Selection.M_Selection"));
	// Check if the mesh asset is found
	if (MaterialAsset.Succeeded())
	{
		SelectedMesh->SetMaterial(0,MaterialAsset.Object);
		UE_LOG(LogTemp,Display,TEXT("FOUND"));
	}

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

	SelectedMesh->AttachToComponent(MeshComponent,FAttachmentTransformRules::SnapToTargetNotIncludingScale);
	SelectedMesh->SetRelativeLocation(FVector(0,0,0));
	SelectedMesh->SetCollisionProfileName("NoCollision");
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

void URTSEntity::SetSelected()
{
	const float Scale = Extent / 50;
	SelectedMesh->SetWorldScale3D(FVector(Scale,Scale,0.1f));
	SelectedMesh->SetVisibility(true);

	UE_LOG(LogTemp,Error,TEXT("Scale : %f"),Scale);
}

void URTSEntity::SetUnSelected()
{
	SelectedMesh->SetVisibility(false);
}



