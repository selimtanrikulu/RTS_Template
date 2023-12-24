// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ActorsAndComponents/RTSEntity.h"
#include "NeutralEntity.generated.h"

/**
 * 
 */
UCLASS()
class RTSPLUGIN_API UNeutralEntity : public URTSEntity
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UNeutralEntity();
	void Init(const FEntityData &entityData);
	
protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;


	
	
	//RTS Entity
	virtual FEntityData GetEntityData() const override;
	virtual FString GetInfo() const override;
	virtual float GetProgress() const override;


private:

	//Config
	FEntityData EntityData;
	
};
