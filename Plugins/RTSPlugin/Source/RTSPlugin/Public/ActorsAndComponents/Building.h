// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TeamEntity.h"
#include "GameFramework/Actor.h"
#include "Managers/SelectionManager.h"
#include "Utility/Util.h"
#include "Building.generated.h"

class UStoreManager;
class UAsset_Manager;
class UTeamEntity;


UENUM(BlueprintType)
enum class EBuildingState : uint8
{
	None,
	Draft,
	Error,
	Construction,
	Located,
};




UCLASS()
class RTSPLUGIN_API ABuilding : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABuilding();
	void Init(const FBuildingData& BuildingData);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;


public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	//Events
	UPROPERTY(BlueprintAssignable) FTeamEntityAction OnBuildingChangedDelegate;
	
	//Components
	UPROPERTY() UTeamEntity* TeamEntity;
	
	
	void SetBuildingState(EBuildingState buildingState);
	void CacheMaterials();
	bool Locatable() const;
	EBuildingState GetBuildingState() const;
	void Progress(float Amount);
	FBuildingData GetBuildingData() const;


	
private:

	//Dependencies
	UPROPERTY() UAsset_Manager* AssetManager;

	//Config
	FBuildingData BuildingData;

	//Components
	UPROPERTY(EditDefaultsOnly) UStaticMeshComponent* StaticMeshComponent;
	
	//Utility
	UPROPERTY() UStaticMesh* ConstructedStaticMesh;
	UPROPERTY() TArray<UMaterialInterface*> CachedMaterialsConstructed;
	UPROPERTY() TArray<UMaterialInterface*> CachedMaterialsLevel1;
	UPROPERTY() TArray<UMaterialInterface*> CachedMaterialsLevel2;
	float Delta_Time;
	
	//State
	EBuildingState BuildingState;
	
	//Utility Functions
	void SetErrorMaterial() const;
	void SetDraftMaterial() const;
	void SetConstructionMesh(int Level) const;
	void SetConstructedMesh();
	
	
};
