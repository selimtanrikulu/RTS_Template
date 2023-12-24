// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ManagerBase.h"
#include "ActorsAndComponents/TeamEntity.h"
#include "UObject/NoExportTypes.h"
#include "Utility/Util.h"
#include "SourceManager.generated.h"

class UStoreManager;
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnSourceAction);

/**
 * 
 */
UCLASS()
class RTSPLUGIN_API USourceManager : public UManagerBase
{
	GENERATED_BODY()

public:

	//Manager Base
	virtual void Init(URTSGameInstance* gameInstance) override;
	virtual void PostInit() override;
	virtual void Begin() override;
	virtual void Tick(float DeltaTime) override;
	//--------------

	FOnSourceAction& GetSourceChangeDelegate(const FString& SourceName);
	int GetSourceAmount(const FString &SourceName);

	void Buy(const FTeamEntityData& TeamEntityData);
	
	bool CheckBalance(const FTeamEntityData& TeamEntityData);
	
private:
	//Dependencies
	UPROPERTY() UStoreManager* StoreManager;

	//Config
	TArray<FCostData> Sources;
	
	TArray<TPair<FString,FOnSourceAction>> OnSourceChangeDelegates;
	FOnSourceAction GarbageAction;

	bool CheckBalance(const FCostData &CostData);
	bool CheckBalance(const FString &SourceName,int Amount);

	void UpdateSource(const FString& SourceName,int Amount);
};
