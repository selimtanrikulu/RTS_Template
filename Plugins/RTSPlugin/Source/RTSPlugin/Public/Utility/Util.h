// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

/**
 * 
 */
class RTSPLUGIN_API Util
{
public:
	Util();
	~Util();


	static AActor* GetActorOfClass(const UObject* WorldContextObject,
							  TSubclassOf<AActor> ActorClass);

	
};
