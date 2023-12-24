// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Utility/Util.h"
#include "UnitGenerator.generated.h"


class USourceManager;
class UEntityManager;
class UStoreManager;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class RTSPLUGIN_API UUnitGenerator : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UUnitGenerator();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;


	TArray<FUnitData>& GetUnits();
	
	void SpawnUnit(const FUnitData& UnitData) const;


private:
	//Dependencies
	UPROPERTY() UStoreManager* StoreManager;
	UPROPERTY() UEntityManager* EntityManager;
	UPROPERTY() USourceManager* SourceManager;

	UPROPERTY() ABuilding* OwnerBuilding;
	

	UPROPERTY(EditDefaultsOnly) TArray<FString> UnitNames;

	TArray<FUnitData> Units;


	//Unit Generation
	bool IsLocationOccupied(const FVector& Location, float Radius) const;
	FVector FindClosestEmptyLocation(float UnitExtent) const;

	

	
};
