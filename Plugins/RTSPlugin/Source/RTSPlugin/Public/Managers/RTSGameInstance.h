// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "RTSGameInstance.generated.h"

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

private:
	
	//Level Calls
	UFUNCTION(BlueprintCallable)
	void OnBeginPlay();

	UFUNCTION(BlueprintCallable)
	void OnTick(float DeltaTime);




	//Tool (To be moved to configs)
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<AActor> SelectBoxBP;
	
};
