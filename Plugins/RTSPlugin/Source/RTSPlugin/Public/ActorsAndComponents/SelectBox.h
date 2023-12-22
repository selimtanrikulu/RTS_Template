// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "..\Managers\SelectionManager.h"
#include "SelectBox.generated.h"

class ABuilding;
class ARTSPawn;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnOverlapChanged);

UCLASS()
class RTSPLUGIN_API ASelectBox : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASelectBox();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;


	TArray<ABuilding*> OverlappingBuildings;
	TArray<AUnit*> OverlappingUnits;
	
	UPROPERTY(BlueprintAssignable) FOnOverlapChanged OnOverlapChangedDelegate;

private:

	//Dependencies
	UPROPERTY() UPlayerManager* PlayerManager;

	
	FVector StartLocation;


	void UpdateLocationAndScale();
	void UpdateOverlaps();
	
	
	
	

	
};
