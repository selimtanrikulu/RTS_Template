// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ActorsAndComponents/Unit.h"
#include "Worker.generated.h"

class USourceHolder;
/**
 * 
 */



UCLASS()
class RTSPLUGIN_API AWorker : public AUnit
{
	GENERATED_BODY()

public:

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;


	//Unit
	virtual void SetTargetRTSEntity(const URTSEntity* Entity) override;
	virtual void SetTargetLocation(const FVector& TargetLocation) override;
	//---------------


	
	
	
private:

	//Config
	float CollectDelay = 1.0f;
	float CollectDelayCounter;
	float WorkerRange = 100;

	//Collect & Construct
	UPROPERTY() USourceHolder* CollectingSource;
	UPROPERTY() ABuilding* ConstructingBuilding;

	//Worker Animation State
	virtual void UpdateUnitAnimation() override;

	//Event Listeners
	//Source Holder
	UFUNCTION() void OnCollectingSourceFinished(USourceHolder* SourceHolder);
	void BindSource(USourceHolder* SourceHolder);
	void UnbindSource();

	//Building
	UFUNCTION() void OnBuildingStateChanged(ABuilding* Building);
	UFUNCTION() void OnBuildingKilled(UTeamEntity* teamEntity2);
	void BindBuilding(ABuilding* Building);
	void UnbindBuilding();
	
	//Utility
	bool IsInRange(const URTSEntity* Entity) const;
	bool IsCollectingSource() const;
	bool IsConstructingBuilding() const;

	
};
