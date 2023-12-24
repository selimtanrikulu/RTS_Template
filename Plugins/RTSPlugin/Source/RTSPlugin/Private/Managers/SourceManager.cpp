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

	TArray<FSourceData> SourcesData = StoreManager->GetSourcesData();
	for(FSourceData &SourceData : SourcesData)
	{
		TPair<FString,FOnSourceAction> SourceDelegatePair;
		SourceDelegatePair.Key = SourceData.EntityData.Name;
		OnSourceChangeDelegates.Add(SourceDelegatePair);
		FCostData Source;
		Source.SourceName = SourceData.EntityData.Name;
		Source.Amount = SourceData.StartAmount;
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

int USourceManager::GetSourceAmount(const FString& SourceName)
{
	for(FCostData &Source : Sources)
	{
		if(Source.SourceName == SourceName)
		{
			return Source.Amount;
		}
	}

	UE_LOG(LogTemp,Error,TEXT("Source cannot be found"));
	return 0;
}



void USourceManager::UpdateSource(const FString& SourceName, const int Amount)
{
	//Update Source
	for(FCostData &Source : Sources)
	{
		if(Source.SourceName == SourceName)
		{
			Source.Amount += Amount;

			if(Source.Amount < 0)
			{
				UE_LOG(LogTemp,Error,TEXT("Source is less than zero after update"));
				Source.Amount = 0;
			}

			break;
		}
	}
	
	//Broadcast Event
	for(TPair<FString,FOnSourceAction> &SourceDelegatePair : OnSourceChangeDelegates)
	{
		if(SourceDelegatePair.Key == SourceName)
		{
			SourceDelegatePair.Value.Broadcast();
		}
	}
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



bool USourceManager::CheckBalance(const FCostData &CostData)
{
	return CheckBalance(CostData.SourceName,CostData.Amount);
}

bool USourceManager::CheckBalance(const FString& SourceName,int Amount)
{
	for(FCostData &Source : Sources)
	{
		if(Source.SourceName == SourceName)
		{
			if(Source.Amount >= Amount)
			{
				return true;
			}
			//else
			return false;
		}
	}


	UE_LOG(LogTemp,Error,TEXT("Source cannot be found to check balance"));
	return false;
}
