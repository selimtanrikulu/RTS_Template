// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ManagerBase.h"
#include "UObject/NoExportTypes.h"
#include "UnitManager.generated.h"

class UPlayerManager;
class USelectionManager;
/**
 * 
 */
UCLASS()
class RTSPLUGIN_API UUnitManager : public UManagerBase
{
	GENERATED_BODY()

	
public:
	
	//Manager Base
	virtual void Init(URTSGameInstance* gameInstance) override;
	virtual void Begin() override;
	virtual void Tick(float DeltaTime) override;
	//--------------


private:
	//Dependencies
	UPROPERTY() UPlayerManager* PlayerManager;
	UPROPERTY() USelectionManager* SelectionManager;


	//Event Listeners
	UFUNCTION() void OnMouseRightClicked();
	
};
