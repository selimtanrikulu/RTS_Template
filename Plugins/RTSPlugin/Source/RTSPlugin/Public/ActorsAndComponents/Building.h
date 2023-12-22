// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Utility/Util.h"
#include "Building.generated.h"

class UStoreManager;
class UAsset_Manager;
class UTeamEntity;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnBuildingAction);

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

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;


public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(BlueprintAssignable) FOnBuildingAction OnBuildingStateChangedDelegate;
	
	UPROPERTY() UTeamEntity* TeamEntity;

	//Config
	FBuildingData BuildingData;
	
	void SetBuildingState(EBuildingState buildingState);
	void CacheMaterials();
	bool Locatable() const;
	EBuildingState GetBuildingState() const;
	float GetProgress() const;

private:

	//Dependencies
	UPROPERTY() UAsset_Manager* AssetManager;


	//Components
	UPROPERTY() UStaticMeshComponent* StaticMeshComponent;
	
	//Utility
	UPROPERTY() UStaticMesh* ConstructedStaticMesh;
	UPROPERTY() TArray<UMaterialInterface*> CachedMaterialsConstructed;
	UPROPERTY() TArray<UMaterialInterface*> CachedMaterialsLevel1;
	UPROPERTY() TArray<UMaterialInterface*> CachedMaterialsLevel2;

	//State
	EBuildingState BuildingState;
	float CurrentWorkerEnergySeconds;


	//Utility Functions
	void SetErrorMaterial() const;
	void SetDraftMaterial() const;
	void SetConstructionMesh(int Level) const;
	void SetConstructedMesh();
	
};
