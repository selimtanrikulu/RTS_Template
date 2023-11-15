// Fill out your copyright notice in the Description page of Project Settings.


#include "ActorsAndComponents/Building.h"

#include "ActorsAndComponents/TeamEntity.h"
#include "Kismet/GameplayStatics.h"
#include "Managers/Asset_Manager.h"
#include "Managers/RTSGameInstance.h"

// Sets default values
ABuilding::ABuilding()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	TeamEntity = CreateDefaultSubobject<UTeamEntity>(TEXT("Team Entity"));
}

// Called when the game starts or when spawned
void ABuilding::BeginPlay()
{
	Super::BeginPlay();

	const APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(),0);
	const URTSGameInstance* GameInstance = Cast<URTSGameInstance>(PlayerController->GetGameInstance());

	AssetManager = GameInstance->AssetManager;


	MeshComponent = FindComponentByClass<UMeshComponent>();

	CacheMaterials();
}

// Called every frame
void ABuilding::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}



void ABuilding::SetError() const
{
	for(int i=0;i<MeshComponent->GetMaterials().Num();i++)
	{
		MeshComponent->SetMaterial(0,AssetManager->GetErrorMaterial());
	}
}

void ABuilding::SetDraft() const
{
	for(int i=0;i<MeshComponent->GetMaterials().Num();i++)
	{
		MeshComponent->SetMaterial(0,AssetManager->GetDraftMaterial());
	}
}

void ABuilding::SetCache()
{
	for(int i=0;i<MeshComponent->GetMaterials().Num();i++)
	{
		MeshComponent->SetMaterial(0,CachedMaterials[i]);
	}
}

void ABuilding::CacheMaterials()
{
	for(int i=0;i<MeshComponent->GetMaterials().Num();i++)
	{
		CachedMaterials.Add(MeshComponent->GetMaterial(i));
	}
}
