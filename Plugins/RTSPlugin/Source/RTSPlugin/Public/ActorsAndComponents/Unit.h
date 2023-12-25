// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Utility/Util.h"
#include "Unit.generated.h"

class URTSEntity;
class UPlayerManager;
class UTeamEntity;
class UAsset_Manager;

UCLASS()
class RTSPLUGIN_API AUnit : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AUnit();
	void Init(const FUnitData &UnitData);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	//Components
	UPROPERTY() UTeamEntity* TeamEntity;


	virtual void SetTargetRTSEntity(const URTSEntity* Entity);
	virtual void SetTargetLocation(const FVector& TargetLocation);

	FUnitData GetUnitData() const;

	
private:

	//Dependencies
	UPROPERTY() UAsset_Manager* AssetManager;
	
	//Config
	FUnitData UnitData;


	

};
