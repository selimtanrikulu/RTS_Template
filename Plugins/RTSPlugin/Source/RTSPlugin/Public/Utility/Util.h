// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "Util.generated.h"


class AUnit;
class ABuilding;

USTRUCT(BlueprintType)
struct FCostData
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly)
	FString SourceName;
	
	UPROPERTY(EditDefaultsOnly)
	int Amount;

	FString GetInfo() const
	{
		FString Info = "";
		Info += SourceName;
		Info += " ";
		Info += FString::FormatAsNumber(Amount);
		Info += "\n";
		return Info;
	}
};

USTRUCT(BlueprintType)
struct FEntityData
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly)
	FString Name;
	
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<AActor> BP;
	
	UPROPERTY()
	UMaterialInstanceDynamic* ImageMaterial;

	UPROPERTY()
	int EntityID;


	FString GetInfo() const
	{
		FString Info = "";
		Info += "Name : ";
		Info += Name;
		Info += "\n";
		Info += "ID : ";
		Info += FString::FormatAsNumber(EntityID);
		Info += "\n";
		return Info;
	}
	
};


USTRUCT(BlueprintType)
struct FSourceData
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly)
	FEntityData EntityData;

	UPROPERTY(EditDefaultsOnly)
	int StartAmount;
	
	FString GetInfo() const
	{
		return EntityData.GetInfo();
	}
};

USTRUCT(BlueprintType)
struct FTeamEntityData
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly)
	FEntityData EntityData;
	
	UPROPERTY(EditDefaultsOnly)
	TArray<FCostData> Cost;

	UPROPERTY(EditDefaultsOnly)
	float HP;

	FString GetInfo() const
	{
		FString Info = "";

		Info += EntityData.GetInfo();

		for(FCostData CostData : Cost)
		{
			Info += CostData.GetInfo();
		}

		return Info;
	}
};



USTRUCT(BlueprintType)
struct FConstructionData
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly)
	UStaticMesh* Level1;

	UPROPERTY(EditDefaultsOnly)
	UStaticMesh* Level2;
	
	UPROPERTY(EditDefaultsOnly)
	float TotalWorkerEnergySeconds;

	FString GetInfo() const
	{
		return "";
	}
};



USTRUCT(BlueprintType)
struct FBuildingData
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly)
	FTeamEntityData TeamEntityData;
	
	UPROPERTY(EditDefaultsOnly)
	FConstructionData ConstructionData;

	UPROPERTY(EditDefaultsOnly)
	FString Path;

	FString GetInfo() const
	{
		FString Info = "";
		Info += TeamEntityData.GetInfo();
		Info += ConstructionData.GetInfo();
		return Info;
	}
	
};





USTRUCT(BlueprintType)
struct FUnitData
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly)
	FTeamEntityData TeamEntityData;

	FString GetInfo() const
	{
		return TeamEntityData.GetInfo();
	}
};



USTRUCT(BlueprintType)
struct FStoreManagerConfig
{
	GENERATED_BODY()
	
	UPROPERTY(EditDefaultsOnly)
	TArray<FBuildingData> BuildingsData;


	UPROPERTY(EditDefaultsOnly)
	TArray<FUnitData> UnitsData;

	UPROPERTY(EditDefaultsOnly)
	TArray<FSourceData> SourcesData;
	
};


USTRUCT(BlueprintType)
struct FAssetManagerConfig
{
	GENERATED_BODY()


	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<AAIController> AIControllerBP;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<AActor> SelectBoxBP;
	
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<AActor> StudioBP;

	UPROPERTY(EditDefaultsOnly)
	UMaterialInterface* DraftMaterial;
	
	UPROPERTY(EditDefaultsOnly)
	UMaterialInterface* ErrorMaterial;
};

/**
 * 
 */
class RTSPLUGIN_API Util
{
public:
	Util();
	~Util();

/*
	static AActor* GetActorOfClass(const UObject* WorldContextObject,
							  TSubclassOf<AActor> ActorClass);


*/
	static TArray<AActor*> GetActorsOfClass(const UObject* WorldContextObject,
							  TSubclassOf<AActor> ActorClass);

	
};
