// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ManagerBase.h"
#include "CursorManager.generated.h"

/**
 * 
 */


class UPlayerManager;
class UMouseCursorWidget;

UENUM(BlueprintType)
enum class EMouseCursorType : uint8
{
	None,
	Default,
	Highlight,
	Attack,
};



UCLASS()
class RTSPLUGIN_API UCursorManager : public UManagerBase
{
	GENERATED_BODY()


public:

	UCursorManager();

	//Manager Base
	virtual void Init(URTSGameInstance* gameInstance) override;
	virtual void Begin() override;
	virtual void Tick(float DeltaTime) override;
	//--------------


private:
	//Dependencies
	UPROPERTY() UPlayerManager* PlayerManager;

	
	//Mouse cursors
	TSubclassOf<UUserWidget> CursorBP;
	UPROPERTY() UMouseCursorWidget* MouseCursorWidget;
	
	EMouseCursorType CurrentMouseCursorType;
	void SetCursor(EMouseCursorType MouseCursorType);

	
};
