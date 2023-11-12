// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Util.generated.h"


class AUnit;
class ABuilding;

USTRUCT(BlueprintType)
struct FSourceData
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly)
	FString SourceName;
	
	UPROPERTY(EditDefaultsOnly)
	int Amount;
};



USTRUCT(BlueprintType)
struct FEntityData
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<AActor> BP;
	
	UPROPERTY(EditDefaultsOnly)
	FString Name;
	
	UPROPERTY(EditDefaultsOnly)
	TArray<FSourceData> Cost;

	UPROPERTY()
	UMaterialInstanceDynamic* ImageMaterial;

	UPROPERTY()
	int EntityID;
	
};


USTRUCT(BlueprintType)
struct FBuildingData
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly)
	FEntityData EntityData;

	UPROPERTY(EditDefaultsOnly)
	FString Path;
	
	
	
};





USTRUCT(BlueprintType)
struct FUnitData
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly)
	FEntityData EntityData;
};



USTRUCT(BlueprintType)
struct FStoreManagerConfig
{
	GENERATED_BODY()
	
	UPROPERTY(EditDefaultsOnly)
	TArray<FBuildingData> BuildingsData;


	UPROPERTY(EditDefaultsOnly)
	TArray<FUnitData> UnitsData;
	
};

/**
 * 
 */
class RTSPLUGIN_API Util
{
public:
	Util();
	~Util();


	static AActor* GetActorOfClass(const UObject* WorldContextObject,
							  TSubclassOf<AActor> ActorClass);



	static TArray<AActor*> GetActorsOfClass(const UObject* WorldContextObject,
							  TSubclassOf<AActor> ActorClass);

	
};
