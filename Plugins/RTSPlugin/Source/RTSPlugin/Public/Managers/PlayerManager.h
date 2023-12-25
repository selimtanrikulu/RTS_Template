// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ManagerBase.h"
#include "UObject/NoExportTypes.h"
#include "PlayerManager.generated.h"

class URTSEntity;
class ARTSPawn;


DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnPlayerManagerAction);

/**
 * 
 */
UCLASS()
class UPlayerManager : public UManagerBase
{
	GENERATED_BODY()
	


public:

	//Manager Base
	virtual void Init(URTSGameInstance* gameInstance) override;
	virtual void Begin() override;
	virtual void Tick(float DeltaTime) override;
	//--------------

	void SetPawn(ARTSPawn* RTSPawn);


	//Input Events
	UPROPERTY(BlueprintAssignable) FOnPlayerManagerAction OnMouseLeftClickDelegate;
	UPROPERTY(BlueprintAssignable) FOnPlayerManagerAction OnMouseLeftReleasedDelegate;
	UPROPERTY(BlueprintAssignable) FOnPlayerManagerAction OnMouseRightClickDelegate;
	UPROPERTY(BlueprintAssignable) FOnPlayerManagerAction OnMouseRightReleasedDelegate;
	UPROPERTY(BlueprintAssignable) FOnPlayerManagerAction OnMouseWheelUpDelegate;
	UPROPERTY(BlueprintAssignable) FOnPlayerManagerAction OnMouseWheelDownDelegate;
	UPROPERTY(BlueprintAssignable) FOnPlayerManagerAction OnDeleteClickDelegate;


	//Pawn Functions
	FHitResult LookForFloor() const;
	URTSEntity* LookForRTSEntity() const;

private:

	//Dependencies
	UPROPERTY() ARTSPawn* RTSPawn;

	//Input Actions
	UFUNCTION() void OnMouseLeftClicked();
	UFUNCTION() void OnMouseLeftReleased();
	UFUNCTION() void OnMouseRightClicked();
	UFUNCTION() void OnMouseRightReleased();
	UFUNCTION() void OnMouseWheelUp();
	UFUNCTION() void OnMouseWheelDown();
	UFUNCTION() void OnDeleteClicked();

	
};
