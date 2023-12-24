// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ManagerBase.h"
#include "EntityManager.generated.h"

class USourceHolder;
class UTeamEntity;
class URTSEntity;
/**
 * 
 */
UCLASS()
class RTSPLUGIN_API UEntityManager : public UManagerBase
{
	GENERATED_BODY()


public:

	//Manager Base
	virtual void Init(URTSGameInstance* gameInstance) override;
	virtual void Begin() override;
	virtual void Tick(float DeltaTime) override;
	//--------------
	
	void AddEntity(URTSEntity* Entity);
	
	
private:
	
	TArray<URTSEntity*> Entities;
	
	UFUNCTION() void OnEntityKilled(UTeamEntity* TeamEntity);
	UFUNCTION() void OnSourceFinished(USourceHolder* SourceHolder);

};
