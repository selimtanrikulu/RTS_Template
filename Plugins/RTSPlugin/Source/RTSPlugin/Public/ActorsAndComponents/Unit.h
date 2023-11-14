// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Utility/Util.h"
#include "Unit.generated.h"

class UAsset_Manager;

UCLASS()
class RTSPLUGIN_API AUnit : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AUnit();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;


	void SetTargetLocation(FVector TargetLocation);
	
	FUnitData UnitData;


	//Components
	UPROPERTY() UMeshComponent* MeshComponent;
	FVector BoxExtent;
	float Extent;

private:

	//Dependencies
	UPROPERTY() UAsset_Manager* AssetManager;

};
