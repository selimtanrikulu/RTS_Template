// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "LogManager.generated.h"

/**
 * 
 */
UCLASS()
class RTSPLUGIN_API ULogManager : public UObject
{
	GENERATED_BODY()

	
public:
	ULogManager();
	~ULogManager();


	void EditorLog(const FText& LogText,ELogVerbosity::Type LogVerbosity);
	
};
