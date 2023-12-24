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
	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>("StaticMeshComponent");

	SetRootComponent(StaticMeshComponent);
}


void ABuilding::Init(const FBuildingData& buildingData)
{
	BuildingData = buildingData;
	TeamEntity->Init(BuildingData.TeamEntityData);
}

// Called when the game starts or when spawned
void ABuilding::BeginPlay()
{
	Super::BeginPlay();
	const APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(),0);
	const URTSGameInstance* GameInstance = Cast<URTSGameInstance>(PlayerController->GetGameInstance());
	AssetManager = GameInstance->AssetManager;
}


// Called every frame
void ABuilding::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	Delta_Time = DeltaTime;

	if(BuildingState == EBuildingState::Construction)
	{
		//30 is the construction speed of a worker per second
		Progress(Delta_Time*30);
	}

}


void ABuilding::SetBuildingState(EBuildingState buildingState)
{
	if(BuildingState == buildingState)
	{
		UE_LOG(LogTemp,Display,TEXT("Building state already set"));
		return;
	}

	UE_LOG(LogTemp,Display,TEXT("State set : %s"),*UEnum::GetValueAsString(buildingState));

	BuildingState = buildingState;

	switch(BuildingState)
	{
		case EBuildingState::Draft:
			StaticMeshComponent->SetCollisionProfileName(TEXT("OverlapAll"));
			SetDraftMaterial();
			break;

		case EBuildingState::Error:
			StaticMeshComponent->SetCollisionProfileName(TEXT("OverlapAll"));
			SetErrorMaterial();
			break;

		case EBuildingState::Construction:
			StaticMeshComponent->SetCollisionProfileName(TEXT("BlockAll"));
			SetConstructionMesh(1);
			break;
			
		case EBuildingState::Located:
			SetConstructedMesh();
			StaticMeshComponent->SetCollisionProfileName(TEXT("BlockAll"));
			TeamEntity->HealFull();
			break;

		default:
			UE_LOG(LogTemp,Error,TEXT("Unknown building state"));
			return;
	}


	OnBuildingChangedDelegate.Broadcast(TeamEntity);
	
}

EBuildingState ABuilding::GetBuildingState() const
{
	return BuildingState;
}

void ABuilding::Progress(float Amount)
{
	const float BeforeProgress = TeamEntity->GetHPRatio();
	
	if(BeforeProgress >= 1 || BuildingState != EBuildingState::Construction)
	{
		UE_LOG(LogTemp,Error,TEXT("Errorrenous call for progress"));
		return;
	}
	
	TeamEntity->Heal(Amount);
	
	const float AfterProgress = TeamEntity->GetHPRatio();
	if(AfterProgress>=0.5f && BeforeProgress <0.5f)
	{
		SetConstructionMesh(2);
	}
	
	OnBuildingChangedDelegate.Broadcast(TeamEntity);

	if(TeamEntity->GetHPRatio() >= 1)
	{
		SetBuildingState(EBuildingState::Located);
	}
}

bool ABuilding::Locatable() const
{
	TArray<AActor*> OverlappingActors;
	GetOverlappingActors(OverlappingActors, UStaticMeshComponent::StaticClass());

	return OverlappingActors.Num() <= 0;
}

void ABuilding::SetErrorMaterial() const
{
	for(int i=0;i<StaticMeshComponent->GetMaterials().Num();i++)
	{
		StaticMeshComponent->SetMaterial(0,AssetManager->GetErrorMaterial());
	}
}

void ABuilding::SetDraftMaterial() const
{
	for(int i=0;i<StaticMeshComponent->GetMaterials().Num();i++)
	{
		StaticMeshComponent->SetMaterial(0,AssetManager->GetDraftMaterial());
	}
}

void ABuilding::SetConstructionMesh(int Level) const
{
	
	UStaticMesh* ConstructionMesh;
	TArray<UMaterialInterface*> CachedMaterialsConstruction;

	if(Level == 1)
	{
		ConstructionMesh = BuildingData.ConstructionData.Level1;
		CachedMaterialsConstruction = CachedMaterialsLevel1;
	}
	else
	{
		ConstructionMesh = BuildingData.ConstructionData.Level2;
		CachedMaterialsConstruction = CachedMaterialsLevel2;
	}
	
	StaticMeshComponent->SetStaticMesh(ConstructionMesh);
	
	for(int i=0;i<StaticMeshComponent->GetMaterials().Num();i++)
	{
		StaticMeshComponent->SetMaterial(0,CachedMaterialsConstruction[i]);
	}
	
}

void ABuilding::SetConstructedMesh()
{
	for(int i=0;i<StaticMeshComponent->GetMaterials().Num();i++)
	{
		StaticMeshComponent->SetMaterial(0,CachedMaterialsConstructed[i]);
	}

	StaticMeshComponent->SetStaticMesh(ConstructedStaticMesh);
}



FBuildingData ABuilding::GetBuildingData() const
{
	return BuildingData;
}



void ABuilding::CacheMaterials()
{
	
	for(int i=0;i<StaticMeshComponent->GetMaterials().Num();i++)
	{
		CachedMaterialsConstructed.Add(StaticMeshComponent->GetMaterial(i));
	}

	ConstructedStaticMesh = StaticMeshComponent->GetStaticMesh();

	const int Level1MaterialSize = BuildingData.ConstructionData.Level1->GetStaticMaterials().Num();
	for(int i=0;i<Level1MaterialSize;i++)
	{
		CachedMaterialsLevel1.Add(BuildingData.ConstructionData.Level1->GetMaterial(i));
	}

	const int Level2MaterialSize = BuildingData.ConstructionData.Level2->GetStaticMaterials().Num();
	for(int i=0;i<Level2MaterialSize;i++)
	{
		CachedMaterialsLevel2.Add(BuildingData.ConstructionData.Level2->GetMaterial(i));
	}
}
