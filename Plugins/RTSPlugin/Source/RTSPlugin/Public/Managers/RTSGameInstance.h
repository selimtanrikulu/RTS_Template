// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "Utility/Util.h"
#include "RTSGameInstance.generated.h"

class UStoreManager;
class ARTSHUD;
class UBuildingManager;
class USelectionManager;
class ULogManager;
/**
 * 
 */

UCLASS()
class RTSPLUGIN_API URTSGameInstance : public UGameInstance
{
	GENERATED_BODY()

protected:

	virtual void Init() override;

public:

	//Managers
	UPROPERTY() ULogManager* LogManager;
	UPROPERTY() USelectionManager* SelectionManager;
	UPROPERTY() UBuildingManager* BuildingManager;
	UPROPERTY() ARTSHUD* RTSHUD;
	UPROPERTY() UStoreManager* StoreManager;

private:
	
	//Level Calls
	UFUNCTION(BlueprintCallable)
	void OnBeginPlay();

	UFUNCTION(BlueprintCallable)
	void OnTick(float DeltaTime);


	//Building Manager Config
	UPROPERTY(EditDefaultsOnly)
	FStoreManagerConfig StoreManagerConfig;


	//Tool (To be moved to configs)
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<AActor> SelectBoxBP;


	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<AActor> StudioBP;
	
	
};
