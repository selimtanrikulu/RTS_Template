// Fill out your copyright notice in the Description page of Project Settings.


#include "Managers/SourceManager.h"

#include "Managers/RTSGameInstance.h"
#include "Managers/StoreManager.h"

void USourceManager::Init(URTSGameInstance* gameInstance)
{
	Super::Init(gameInstance);
	
	StoreManager = GameInstance->StoreManager;

	
}

void USourceManager::Begin()
{
	Super::Begin();
}

void USourceManager::PostInit()
{
	Super::PostInit();

	TArray<FSourceInfo> SourcesInfo = StoreManager->GetSourcesInfo();
	for(FSourceInfo &SourceInfo : SourcesInfo)
	{
		TPair<FString,FOnSourceAction> SourceDelegatePair;
		SourceDelegatePair.Key = SourceInfo.Name;
		OnSourceChangeDelegates.Add(SourceDelegatePair);
		FCostData Source;
		Source.SourceName = SourceInfo.Name;
		Source.Amount = SourceInfo.StartAmount;
		Sources.Add(Source);
	}
}

void USourceManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

FOnSourceAction& USourceManager::GetSourceChangeDelegate(const FString& SourceName)
{
	for(TPair<FString,FOnSourceAction> &SourceDelegatePair : OnSourceChangeDelegates)
	{
		if(SourceDelegatePair.Key == SourceName)
		{
			return SourceDelegatePair.Value;
		}
	}

	UE_LOG(LogTemp,Error,TEXT("Source Delegate cannot be found"));
	return GarbageAction;
}


FCostData& USourceManager::GetSource(const FString& SourceName)
{
	for(FCostData &Source : Sources)
	{
		if(Source.SourceName == SourceName)
		{
			return Source;
		}
	}

	UE_LOG(LogTemp,Error,TEXT("Source cannot be found"));
	return GarbageSource;
}


int USourceManager::GetSourceAmount(const FString& SourceName)
{
	const FCostData &Source = GetSource(SourceName);
	return Source.Amount;
}



void USourceManager::UpdateSource(const FString& SourceName, const int Amount)
{
	//Update Source
	FCostData &Source = GetSource(SourceName);
	Source.Amount += Amount;
	if(Source.Amount < 0)
	{
		UE_LOG(LogTemp,Error,TEXT("Source is less than zero after update"));
		Source.Amount = 0;
	}
	
	//Broadcast Event
	const FOnSourceAction SourceActionDelegate = GetSourceChangeDelegate(SourceName);
	SourceActionDelegate.Broadcast();
	
}

void USourceManager::Buy(const FTeamEntityData& TeamEntityData)
{
	for(const FCostData &CostData : TeamEntityData.Cost)
	{
		UpdateSource(CostData.SourceName,-CostData.Amount);
	}
}

bool USourceManager::CheckBalance(const FTeamEntityData& TeamEntityData)
{
	for(const FCostData &CostData : TeamEntityData.Cost)
	{
		if(!CheckBalance(CostData))return false;
	}

	return true;
}

void USourceManager::Earn(const FString& SourceName, const int Amount)
{
	UpdateSource(SourceName,Amount);
}


bool USourceManager::CheckBalance(const FCostData &CostData)
{
	return CheckBalance(CostData.SourceName,CostData.Amount);
}

bool USourceManager::CheckBalance(const FString& SourceName,int Amount)
{
	const FCostData &Source = GetSource(SourceName);
	return Source.Amount >= Amount;
}
