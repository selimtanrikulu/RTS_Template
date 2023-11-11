// Fill out your copyright notice in the Description page of Project Settings.


#include "Managers/LogManager.h"
#include "Logging/LogVerbosity.h"


ULogManager::ULogManager()
{
}

ULogManager::~ULogManager()
{
}

void ULogManager::EditorLog(const FText& LogText,ELogVerbosity::Type LogVerbosity)
{
	// Use the appropriate log category based on the verbosity
	switch (LogVerbosity)
	{
		case ELogVerbosity::Fatal:
			UE_LOG(LogTemp, Fatal, TEXT("%s"), *LogText.ToString());
			break;
		case ELogVerbosity::Error:
			UE_LOG(LogTemp, Error, TEXT("%s"), *LogText.ToString());
			break;
		case ELogVerbosity::Warning:
			UE_LOG(LogTemp, Warning, TEXT("%s"), *LogText.ToString());
			break;
		case ELogVerbosity::Display:
			UE_LOG(LogTemp, Display, TEXT("%s"), *LogText.ToString());
			break;
		case ELogVerbosity::Log:
			UE_LOG(LogTemp, Log, TEXT("%s"), *LogText.ToString());
			break;
		case ELogVerbosity::Verbose:
			UE_LOG(LogTemp, Verbose, TEXT("%s"), *LogText.ToString());
			break;
		case ELogVerbosity::VeryVerbose:
			UE_LOG(LogTemp, VeryVerbose, TEXT("%s"), *LogText.ToString());
			break;
		default:
			// Use Log category by default
			UE_LOG(LogTemp, Log, TEXT("%s"), *LogText.ToString());
			break;
	}
}
