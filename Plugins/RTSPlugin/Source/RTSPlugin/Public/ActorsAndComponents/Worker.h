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
	float CollectRange = 100;

	//State
	UPROPERTY() USourceHolder* CollectingSource;

	//Event Listeners
	UFUNCTION() void OnCollectingSourceFinished(USourceHolder* SourceHolder);
	void BindSource(USourceHolder* SourceHolder);
	void UnbindSource();

	//Utility
	bool IsCollectingSourceInRange() const;
	
};
