// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ManagerBase.generated.h"

class URTSGameInstance;
/**
 * 
 */
UCLASS()
class UManagerBase : public UObject
{
	GENERATED_BODY()



public:


	virtual void Init(URTSGameInstance* gameInstance);
	virtual void PostInit();
	virtual void Begin();
	virtual void Tick(float DeltaTime);


protected:

	UPROPERTY() URTSGameInstance* GameInstance;
	
};
