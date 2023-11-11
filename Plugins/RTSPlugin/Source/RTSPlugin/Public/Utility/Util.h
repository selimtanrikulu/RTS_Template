// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Util.generated.h"


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
struct FBuildingData
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<AActor> BP;

	UPROPERTY(EditDefaultsOnly)
	FString Path;
	
	UPROPERTY(EditDefaultsOnly)
	FString Name;
	
	UPROPERTY(EditDefaultsOnly)
	TArray<FSourceData> Cost;
	
};



USTRUCT(BlueprintType)
struct FBuildingManagerConfig
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly)
	TArray<FBuildingData> BuildingsData;
	
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

	
};
