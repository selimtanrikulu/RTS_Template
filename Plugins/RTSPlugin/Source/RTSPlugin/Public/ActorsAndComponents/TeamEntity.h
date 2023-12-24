// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RTSEntity.h"
#include "Components/ActorComponent.h"
#include "Utility/Util.h"
#include "TeamEntity.generated.h"


class UEntityManager;
class UAsset_Manager;


DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FTeamEntityAction, UTeamEntity*, TeamEntity);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class RTSPLUGIN_API UTeamEntity : public URTSEntity
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UTeamEntity();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	//Events
	FTeamEntityAction OnTeamEntityGetDamageDelegate;

		
	virtual FEntityData GetEntityData() const override;
	virtual FString GetInfo() const override;
	virtual float GetProgress() const override;
	
	void Init(float CurrentHP);
	void ApplyDamage(float Damage);

private:



	float GetHPRatio() const;
	float CurrentHP;
	

};
