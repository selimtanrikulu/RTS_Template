// Fill out your copyright notice in the Description page of Project Settings.

#pragma once



#include "CoreMinimal.h"
#include "USelectionManager.generated.h"

class ABuilding;
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnSelectionChanged);


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


	TArray<ABuilding*> GetSelectedBuildings() const;

	UPROPERTY(BlueprintAssignable) FOnSelectionChanged OnSelectionChangedDelegate;

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

	//Select Box Listeners
	UFUNCTION()
	void OnOverlapChanged();



	
	TSubclassOf<AActor> SelectBoxBP;


	//Utility
	UPROPERTY() ASelectBox* CurrentSelectBox;


	TArray<ABuilding*> SelectedBuildings;
	
	
	void CreateSelectBox();
	void DestroySelectBox();
	
};
