// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "SourceHolder.generated.h"


class UEntityManager;
class UNeutralEntity;
class USourceManager;


DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FSourceAction, USourceHolder*, SourceHolder);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class RTSPLUGIN_API USourceHolder : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	USourceHolder();



protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	
	
	//Events
	FSourceAction OnSourceCollectedDelegate;
	FSourceAction OnSourceFinishedDelegate;
	

	void Collect();
	float GetProgress() const;
	FString GetInfo() const;

	
private:

	//Dependencies
	UPROPERTY() USourceManager* SourceManager;



	
	//Config
	UPROPERTY(EditDefaultsOnly)
	FString SourceName;
	
	UPROPERTY(EditDefaultsOnly)
	int HoldAmount;

	

	//State
	int AmountLeft;

};
