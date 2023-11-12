// Fill out your copyright notice in the Description page of Project Settings.


#include "RTSPlugin/Public/Managers/StoreManager.h"

#include "Components/DirectionalLightComponent.h"
#include "Components/SceneCaptureComponent2D.h"
#include "Engine/TextureRenderTarget2D.h"
#include "Kismet/GameplayStatics.h"
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

// Called when the game starts or when spawned
void UStoreManager::Begin(const FStoreManagerConfig &storeManagerConfig,
			   TSubclassOf<AActor> studioBP,
			   UWorld* world)
{
	World = world;
	StoreManagerConfig = storeManagerConfig;
	StudioBP = studioBP;


	APlayerController* PlayerController = UGameplayStatics::GetPlayerController(World,0);
	URTSGameInstance* GameInstance = Cast<URTSGameInstance>(PlayerController->GetGameInstance());

	
	


	
	//Get dependencies
	BuildingManager = GameInstance->BuildingManager;
	//------


	CreateImages();
	CreateStoreTree();



	//Temporarily
	GameInstance->RTSHUD->SetWidget(EWidgetType::Main);
	
}



void UStoreManager::CreateImages()
{
	if (!BaseMaterial)
	{
		UE_LOG(LogTemp, Error, TEXT("Base Material For Scene Capture Cannot Be Found"));
		return;
	}

	if (!StudioBP)
	{
		UE_LOG(LogTemp, Error, TEXT("Camera studio cannot be found"));
		return;
	}


	//Create Studio
	const FVector SpawnLocation(0, 0, 5000);
	AActor* Studio = World->SpawnActor<AActor>(StudioBP, SpawnLocation, FRotator(0));
	

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


	TArray<FEntityData*> EntitiesToCapture;

	for(FBuildingData &BuildingData : StoreManagerConfig.BuildingsData)
	{
		EntitiesToCapture.Add(&BuildingData.EntityData);
	}
	for(FUnitData &UnitData : StoreManagerConfig.UnitsData)
	{
		EntitiesToCapture.Add(&UnitData.EntityData);
	}

	

	for (FEntityData* EntityData : EntitiesToCapture)
	{
		//Spawn Grid Object
		TSubclassOf<AActor> EntityBP = EntityData->BP;
		AActor* SpawnedActor = World->SpawnActor<AActor>(EntityBP, SpawnLocation, FRotator(0));


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
		MeshComponent->SetRelativeRotation(FRotator());

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
		FVector Location = EntityActor->GetActorLocation();

		FVector DeltaMesh(0);
		//If mesh is not the root
		if(MeshComponent->GetAttachmentRoot() != Cast<USceneComponent>(MeshComponent))
		{
			DeltaMesh = MeshComponent->GetRelativeLocation();
		}
		
		
		EntityActor->SetActorLocation(Location + FVector(0, 0, -BoxExtent2.Z) - DeltaMesh);

	
		
		//Capture and save
		CaptureComponent->TextureTarget = RenderTarget;
		CaptureComponent->CaptureScene();
		UMaterialInstanceDynamic* MaterialInstance = UMaterialInstanceDynamic::Create(BaseMaterial, this);
		MaterialInstance->SetTextureParameterValue("TextureSampleParameter", RenderTarget);

		EntityData->ImageMaterial = MaterialInstance;

		
		//Restore
		World->DestroyActor(SpawnedActor);
	}


	//Restore
	CaptureComponent->DestroyComponent();
	World->DestroyActor(Studio);
}

void UStoreManager::CreateStoreTree()
{
	StoreTreeRoot = new StoreTree(new StoreNode(TEXT("~")), nullptr);
	CurrentTree = StoreTreeRoot;

	
	for (FBuildingData BuildingData : StoreManagerConfig.BuildingsData)
	{
		TArray<FString> Path;
		BuildingData.Path.ParseIntoArray(Path, TEXT("/"));
		StoreTreeRoot->AddCategory(Path, BuildingData);
	}
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

TArray<FUnitData> UStoreManager::GetCurrentUnits() const
{
	if (!CurrentTree)
	{
		UE_LOG(LogTemp, Error, TEXT("Current Tree not found"))
	}

	return StoreManagerConfig.UnitsData;
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
		StopShopping();
		return;
	}

	CurrentTree = CurrentTree->Parent;

}
void UStoreManager::SetDraftingBuilding(FBuildingData& BuildingData)
{
	//On grid object selected
	StopShopping();
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
void UStoreManager::StartShopping()
{
	CurrentTree = StoreTreeRoot;
	
}
void UStoreManager::StopShopping()
{
	CurrentTree = StoreTreeRoot;
}
