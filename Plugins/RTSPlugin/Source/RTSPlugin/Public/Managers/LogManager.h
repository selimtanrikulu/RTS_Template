// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Managers/ManagerBase.h"
#include "LogManager.generated.h"

/**
 * 
 */
UCLASS()
class RTSPLUGIN_API ULogManager : public UManagerBase
{
	GENERATED_BODY()

	
public:
	ULogManager();
	~ULogManager();

	//Manager Base
	virtual void Init(URTSGameInstance* gameInstance) override;
	virtual void Begin() override;
	virtual void Tick(float DeltaTime) override;
	//--------------


	void EditorLog(const FText& LogText,ELogVerbosity::Type LogVerbosity);
	
};
