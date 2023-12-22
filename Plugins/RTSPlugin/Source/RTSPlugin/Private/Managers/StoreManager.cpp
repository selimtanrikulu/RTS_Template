// Fill out your copyright notice in the Description page of Project Settings.


#include "RTSPlugin/Public/Managers/StoreManager.h"

#include "ActorsAndComponents/Building.h"
#include "ActorsAndComponents/Unit.h"
#include "Components/DirectionalLightComponent.h"
#include "Components/SceneCaptureComponent2D.h"
#include "Engine/TextureRenderTarget2D.h"
#include "Kismet/GameplayStatics.h"
#include "Managers/Asset_Manager.h"
#include "Managers/BuildingManager.h"
#include "Managers/RTSGameInstance.h"
#include "Managers/RTSHUD.h"
#include "Utility/StoreTree.h"
#include "Utility/StoreNode.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "Materials/Material.h"
#include "UObject/ConstructorHelpers.h"


// Sets default values
UStoreManager::UStoreManager()
{
	static ConstructorHelpers::FObjectFinder<UMaterial> MaterialAsset(
		TEXT("Material'/RTSPlugin/Materials/Capture_Mat_DontDelete'"));
	if (MaterialAsset.Succeeded())
	{
		BaseMaterial = MaterialAsset.Object;
	}
	

}

void UStoreManager::Init(URTSGameInstance* gameInstance)
{
	Super::Init(gameInstance);
	
	StoreManagerConfig = GameInstance->StoreManagerConfig;
	
	//Get dependencies
	BuildingManager = GameInstance->BuildingManager;
	AssetManager = GameInstance->AssetManager;
	//---------------
}

void UStoreManager::Begin()
{
	Super::Begin();

	SetIDs();
	CreateImages();
	FindExistingEntities();
	CreateStoreTree();
	
	//Temporarily
	GameInstance->RTSHUD->SetWidget(EWidgetType::Main);
}

void UStoreManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}



void UStoreManager::SetIDs()
{
	int NextID = 1;

	TArray<FEntityData*> EntitiesData = GetEntitiesData();
	
	for(FEntityData* EntityData : EntitiesData)
	{
		EntityData->EntityID = NextID++;
	}
}

void UStoreManager::FindExistingEntities()
{
	for(const FBuildingData &BuildingData : StoreManagerConfig.BuildingsData)
	{
		const TSubclassOf<AActor> ActorClass = BuildingData.EntityData.BP;
		TArray<AActor*> FoundActors = Util::GetActorsOfClass(GameInstance->World,ActorClass);

		for(AActor* FoundActor : FoundActors)
		{
			ABuilding* Building = Cast<ABuilding>(FoundActor);

			if(!Building)
			{
				UE_LOG(LogTemp,Error,TEXT("Found actor is not a building"));
				continue;
			}

			Building->BuildingData = BuildingData;
			Building->CacheMaterials();
			Building->SetBuildingState(EBuildingState::Located);
		}
	}

	for(const FUnitData &UnitData : StoreManagerConfig.UnitsData)
	{
		const TSubclassOf<AActor> ActorClass = UnitData.EntityData.BP;
		TArray<AActor*> FoundActors = Util::GetActorsOfClass(GameInstance->World,ActorClass);
		
		for(AActor* FoundActor : FoundActors)
		{
			AUnit* Unit = Cast<AUnit>(FoundActor);

			if(!Unit)
			{
				UE_LOG(LogTemp,Error,TEXT("Found actor is not a building"));
				continue;
			}

			Unit->UnitData = UnitData;
		}
	}
}

void UStoreManager::CreateImages()
{
	if (!BaseMaterial)
	{
		UE_LOG(LogTemp, Error, TEXT("Base Material For Scene Capture Cannot Be Found"));
		return;
	}

	//Create Studio
	const FVector SpawnLocation(0, 0, 5000);
	AActor* Studio = GameInstance->World->SpawnActor<AActor>(AssetManager->GetStudioBP(), SpawnLocation, FRotator(0));
	

	USceneCaptureComponent2D* CaptureComponent = nullptr;
	UDirectionalLightComponent* DirectionalLightComponent = nullptr;
	
	TArray<UActorComponent*> Components;
	Studio->GetComponents(Components);

	for(UActorComponent* ActorComponent : Components)
	{

		if(ActorComponent->GetName() == TEXT("SceneCapture"))
		{
			CaptureComponent = Cast<USceneCaptureComponent2D>(ActorComponent);
		}
		else if(ActorComponent->GetName() == TEXT("DirectionalLight"))
		{
			DirectionalLightComponent = Cast<UDirectionalLightComponent>(ActorComponent);
		}
		
	}

	if(!CaptureComponent || !DirectionalLightComponent)
	{
		UE_LOG(LogTemp,Error,TEXT("Capture components not found"));
		return;
	}


	TArray<FEntityData*> EntitiesToCapture = GetEntitiesData();

	for (FEntityData* EntityData : EntitiesToCapture)
	{
		//Spawn Grid Object
		TSubclassOf<AActor> EntityBP = EntityData->BP;
		AActor* SpawnedActor = GameInstance->World->SpawnActor<AActor>(EntityBP, SpawnLocation, FRotator(0));


		// Create a Texture Render Target to capture the image
		UTextureRenderTarget2D* RenderTarget = NewObject<UTextureRenderTarget2D>();
		RenderTarget->InitAutoFormat(75, 75);
		RenderTarget->UpdateResourceImmediate();


		//Configure Grid Object Transform
		UMeshComponent* MeshComponent = SpawnedActor->FindComponentByClass<UMeshComponent>();
		if (!MeshComponent)
		{
			UE_LOG(LogTemp, Error, TEXT("Grid Object Does not have a mesh"));
			continue;
		}


		AActor* EntityActor = MeshComponent->GetOwner();

		MeshComponent->SetCastShadow(false);
		//MeshComponent->SetRelativeRotation(FRotator());

		const FVector BoxExtent1 = MeshComponent->Bounds.BoxExtent;
		
		
		float MaxExtent;
		if (BoxExtent1.Y >= BoxExtent1.Z && BoxExtent1.Y >= BoxExtent1.X)MaxExtent = BoxExtent1.Y;
		else if (BoxExtent1.Z >= BoxExtent1.X)MaxExtent = BoxExtent1.Z;
		else MaxExtent = BoxExtent1.X;

		const FVector FScale = EntityActor->GetActorScale();
		const float ScaleFactor = 350 / MaxExtent;
		float Scale = FScale.X;
		Scale *= ScaleFactor;

		EntityActor->SetActorScale3D(FVector(Scale, Scale, Scale));

		
		const FVector BoxExtent2 = MeshComponent->Bounds.BoxExtent;


		
		// Get the camera location and rotation
		FVector CameraLocation = CaptureComponent->GetComponentLocation();

		// Calculate the forward and right vectors based on the camera's rotation
		FVector ForwardVector = CaptureComponent->GetForwardVector();


		const float Distance = CaptureComponent->GetRelativeLocation().X;

		// Calculate the horizontal and vertical extents of the view
		const float HalfFOV = FMath::DegreesToRadians(CaptureComponent->FOVAngle) / 2.0f;
		const float TanHalfFOV = FMath::Tan(HalfFOV);
		const float VerticalExtent = TanHalfFOV * Distance; // 1000 units is the distance from the camera


		
		// Calculate the middle-bottom point
		FVector MiddleBottomPoint = CameraLocation +
			ForwardVector * Distance -
					FVector::UpVector * VerticalExtent;

		const float ForwardBias = BoxExtent2.X / 2;
		FVector Bias(0,0,ForwardBias);
		
		MeshComponent->SetWorldLocation(MiddleBottomPoint + Bias);
		
		//Capture and save
		CaptureComponent->TextureTarget = RenderTarget;
		CaptureComponent->CaptureScene();
		UMaterialInstanceDynamic* MaterialInstance = UMaterialInstanceDynamic::Create(BaseMaterial, this);
		MaterialInstance->SetTextureParameterValue("TextureSampleParameter", RenderTarget);

		EntityData->ImageMaterial = MaterialInstance;

		
		//Restore
		GameInstance->World->DestroyActor(SpawnedActor);
	}


	//Restore
	CaptureComponent->DestroyComponent();
	GameInstance->World->DestroyActor(Studio);
}

void UStoreManager::CreateStoreTree()
{
	StoreTreeRoot = new StoreTree(new StoreNode(TEXT("~")), nullptr);
	CurrentTree = StoreTreeRoot;

	
	for (FBuildingData &BuildingData : StoreManagerConfig.BuildingsData)
	{
		TArray<FString> Path;
		BuildingData.Path.ParseIntoArray(Path, TEXT("/"));
		StoreTreeRoot->AddCategory(Path, BuildingData);
	}
}


TArray<FEntityData*> UStoreManager::GetEntitiesData()
{
	TArray<FEntityData*> EntitiesData;
	
	for(FBuildingData &BuildingData : StoreManagerConfig.BuildingsData)
	{
		EntitiesData.Add(&BuildingData.EntityData);
	}
	for(FUnitData &UnitData : StoreManagerConfig.UnitsData)
	{
		EntitiesData.Add(&UnitData.EntityData);
	}

	return EntitiesData;
}

FBuildingData* UStoreManager::GetBuildingByName(const FString& BuildingName)
{
	for(FBuildingData &BuildingData : StoreManagerConfig.BuildingsData)
	{
		if(BuildingData.EntityData.Name == BuildingName)
		{
			return &BuildingData;
		}
	}

	UE_LOG(LogTemp,Error,TEXT("Building with name : %s does not exist "),*(BuildingName));

	return nullptr;
}

FUnitData* UStoreManager::GetUnitByName(const FString& UnitName)
{
	for(FUnitData &UnitData : StoreManagerConfig.UnitsData)
	{
		if(UnitData.EntityData.Name == UnitName)
		{
			return &UnitData;
		}
	}

	UE_LOG(LogTemp,Error,TEXT("Unit with name : %s does not exist "),*(UnitName));

	return nullptr;
}

TArray<FString> UStoreManager::GetCurrentChildren() const
{
	if (!CurrentTree)
	{
		UE_LOG(LogTemp, Error, TEXT("Current Tree not found"))
		TArray<FString> CurrentChildren;
		return CurrentChildren;
	}

	return CurrentTree->GetChildrenNames();
}
TArray<FBuildingData> UStoreManager::GetCurrentBuildings() const
{
	if (!CurrentTree)
	{
		UE_LOG(LogTemp, Error, TEXT("Current Tree not found"))
	}

	return CurrentTree->Node->ContainingBuildings;
}


TArray<FString> UStoreManager::GetCurrentPath() const
{
	TArray<FString> CurrentPathHelper;

	const StoreTree* Current = CurrentTree;
	
	while(Current)
	{
		CurrentPathHelper.Add(Current->Node->Name);
		Current = Current->Parent;
	}

	TArray<FString> CurrentPath;

	for(int i=CurrentPathHelper.Num()-1 ; i>=0 ;i--)
	{
		CurrentPath.Add(CurrentPathHelper[i]);
	}
	
	return CurrentPath;
}
void UStoreManager::SelectNextCategory(const FString& Category)
{
	StoreTree* ChildTree = CurrentTree->GetChildWithName(Category);

	if (!ChildTree)
	{
		UE_LOG(LogTemp, Error, TEXT("Current path does not contain the category"));
		return;
	}

	CurrentTree = ChildTree;
	
	
}
void UStoreManager::SelectPreviousCategory()
{
	UE_LOG(LogTemp, Display, TEXT("Selecting Previous Category"));


	if (CurrentTree == StoreTreeRoot)
	{
		UE_LOG(LogTemp, Display, TEXT("Already in root category"));
		CloseWorker();
		return;
	}

	CurrentTree = CurrentTree->Parent;

}
void UStoreManager::SetDraftingBuilding(FBuildingData& BuildingData)
{
	//On grid object selected
	CloseWorker();
	BuildingManager->DraftBuilding(BuildingData);
	DraftingBuilding = &BuildingData;
}
void UStoreManager::BuyDraftingObject() const
{
	if(DraftingBuilding)
	{
		//PlayerManager->Buy(*DraftingBuilding);
	}
}
void UStoreManager::CancelDraftingObject()
{
	DraftingBuilding = nullptr;
}
bool UStoreManager::EndOfTree() const
{
	return CurrentTree->Children.Num() == 0;
}
void UStoreManager::OpenWorkerRoot()
{
	CurrentTree = StoreTreeRoot;
	
}
void UStoreManager::CloseWorker()
{
	CurrentTree = StoreTreeRoot;
}
