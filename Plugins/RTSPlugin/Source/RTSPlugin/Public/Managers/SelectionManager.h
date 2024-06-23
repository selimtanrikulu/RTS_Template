// Fill out your copyright notice in the Description page of Project Settings.

#pragma once



#include "CoreMinimal.h"
#include "ActorsAndComponents/Building.h"
#include "ActorsAndComponents/RTSEntity.h"
#include "ActorsAndComponents/SourceHolder.h"
#include "ActorsAndComponents/TeamEntity.h"
#include "ActorsAndComponents/UnitGenerator.h"
#include "Managers/ManagerBase.h"
#include "SelectionManager.generated.h"

class FSourceHolderAction;
class ASource;
class USourceHolder;
class UTeamEntity;
class UPlayerManager;
class UAsset_Manager;
class AUnit;
class ABuilding;
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FSelectionAction);



class ULogManager;
class ASelectBox;
class ARTSPawn;
/**
 * 
 */


UENUM(BlueprintType)
enum class ESelectionState : uint8
{
	None,
	UnitSingle,
	UnitMass,
	UnitMixed,
	BuildingSingle,
	BuildingMass,
	BuildingMixed,
	SourceSingle,
	SourceMass,
	SourceMixed,
	AllMixed,
};





UCLASS()
class RTSPLUGIN_API USelectionManager : public UManagerBase
{
	GENERATED_BODY()

public:
	USelectionManager();
	~USelectionManager();

	//Manager Base
	virtual void Init(URTSGameInstance* gameInstance) override;
	virtual void Begin() override;
	virtual void Tick(float DeltaTime) override;
	//--------------


	TArray<ABuilding*> GetSelectedBuildings() const;
	TArray<AUnit*> GetSelectedUnits() const;
	TArray<ASource*> GetSelectedSources() const;
	TArray<UTeamEntity*> GetSelectedTeamEntities() const;
	TArray<URTSEntity*> GetSelectedRTSEntities() const;

	ESelectionState GetSelectionState() const;
	

	//Events
	FSelectionAction OnSelectedEntitiesChangedDelegate;
	FTeamEntityAction OnTeamEntityHPChangedDelegate;
	FBuildingAction OnBuildingStateChangedDelegate;
	FSourceHolderAction OnSourceCollectedDelegate;
	FOnUnitGenerationAction OnUnitGenerationProgressUpdatedDelegate;
	FOnUnitGenerationAction OnUnitQueueUpdatedDelegate;
	//------------------
	
private:

	//Dependencies
	UPROPERTY() UPlayerManager* PlayerManager;
	UPROPERTY() ULogManager* LogManager;
	UPROPERTY() UAsset_Manager* AssetManager;
	

	//Input listeners
	UFUNCTION() void OnMouseLeftClicked();
	UFUNCTION() void OnMouseLeftReleased();
	UFUNCTION() void OnDeleteClicked();

	//Select Box Listeners
	UFUNCTION() void OnOverlapChanged();
	UFUNCTION() void OnTeamEntityKilled(UTeamEntity* TeamEntity);
	UFUNCTION() void OnTeamEntityHPChanged(UTeamEntity* TeamEntity);
	UFUNCTION() void OnBuildingStateChanged(ABuilding* Building);
	UFUNCTION() void OnSourceCollected(USourceHolder* SourceHolder);
	UFUNCTION() void OnSourceFinished(USourceHolder* SourceHolder);
	UFUNCTION() void OnUnitGenerationProgressUpdated();
	UFUNCTION() void OnUnitQueueChanged();

	void UpdateCircles();
	void UpdateSelectionState();
	

	//Utility
	UPROPERTY() ASelectBox* CurrentSelectBox;

	ESelectionState SelectionState;


	TArray<ABuilding*> SelectedBuildings;
	TArray<AUnit*> SelectedUnits;
	TArray<ASource*> SelectedSources;
	

	void BindSelections();
	void UnbindSelections();
	

	
	void CreateSelectBox();
	void DestroySelectBox();

	//Utility functions
	bool AreSameBuildings(TArray<ABuilding*> Buildings);
	bool AreSameUnits(TArray<AUnit*> Units);
	bool AreSameSources(TArray<ASource*> Sources);
	
	
};
