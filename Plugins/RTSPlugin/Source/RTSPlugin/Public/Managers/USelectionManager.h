// Fill out your copyright notice in the Description page of Project Settings.

#pragma once



#include "CoreMinimal.h"
#include "USelectionManager.generated.h"


class ULogManager;
class ASelectBox;
class ARTSPawn;
/**
 * 
 */


UCLASS()
class RTSPLUGIN_API USelectionManager : public UObject
{
	GENERATED_BODY()

public:
	USelectionManager();
	~USelectionManager();

	void Begin(UWorld* world,TSubclassOf<AActor> selectBoxBP);
	void Tick(float DeltaTime);

private:

	//Dependencies
	UPROPERTY() UWorld* World;
	UPROPERTY() ARTSPawn* RTSPawn;
	UPROPERTY() ULogManager* LogManager;
	

	//Input listeners
	UFUNCTION()
	void OnMouseLeftClicked();
	UFUNCTION()
	void OnMouseLeftReleased();
	
	TSubclassOf<AActor> SelectBoxBP;


	//Utility
	UPROPERTY() ASelectBox* CurrentSelectBox;
	
	
	void CreateSelectBox();
	void DestroySelectBox();
	
};
