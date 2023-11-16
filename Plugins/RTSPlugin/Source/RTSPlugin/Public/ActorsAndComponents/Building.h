// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Utility/Util.h"
#include "Building.generated.h"

class UAsset_Manager;
class UTeamEntity;

UENUM(BlueprintType)
enum class EBuildingState : uint8
{
	None,
	Draft,
	Error,
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
	
	FBuildingData BuildingData;


	UPROPERTY() UTeamEntity* TeamEntity;


	void SetBuildingState(EBuildingState buildingState);

	EBuildingState GetBuildingState() const;
	
	bool Locatable() const;


private:

	//Dependencies
	UPROPERTY() UAsset_Manager* AssetManager;

	UPROPERTY() UStaticMeshComponent* StaticMeshComponent;
	UPROPERTY() TArray<UMaterialInterface*> CachedMaterials;



	EBuildingState BuildingState;
	void SetError() const;
	void SetDraft() const;
	void SetCache();
	void CacheMaterials();
};
