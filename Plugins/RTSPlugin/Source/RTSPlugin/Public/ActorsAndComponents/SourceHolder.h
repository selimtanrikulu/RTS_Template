// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RTSEntity.h"
#include "Components/ActorComponent.h"
#include "Utility/Util.h"
#include "SourceHolder.generated.h"


class UEntityManager;
class UNeutralEntity;
class USourceManager;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class RTSPLUGIN_API USourceHolder : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	USourceHolder();

	
	void Init(const FSourceData &SourceData);

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	//Components
	UPROPERTY() UNeutralEntity* NeutralEntity;
	
	//Events
	FEntityAction OnEntityChangedDelegate;
	
	
	FSourceData GetSourceData() const;
	void Collect();
	float GetProgress() const;
	FString GetInfo() const;

private:

	//Dependencies
	UPROPERTY() USourceManager* SourceManager;
	
	//Config
	FSourceData SourceData;

	//State
	int AmountLeft;
	
};
