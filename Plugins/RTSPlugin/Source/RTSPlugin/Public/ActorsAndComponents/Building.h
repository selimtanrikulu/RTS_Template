// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Utility/Util.h"
#include "Building.generated.h"

class UAsset_Manager;
class UTeamEntity;

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
	
	FBuildingData BuildingData;


	UPROPERTY() UTeamEntity* TeamEntity;

	

	void SetError() const;
	void SetDraft() const;
	void SetCache();
	void CacheMaterials();


private:

	//Dependencies
	UPROPERTY() UAsset_Manager* AssetManager;

	UPROPERTY() UMeshComponent* MeshComponent;
	UPROPERTY() TArray<UMaterialInterface*> CachedMaterials;
	
};
