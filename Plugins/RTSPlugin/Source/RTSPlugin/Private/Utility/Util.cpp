// Fill out your copyright notice in the Description page of Project Settings.


#include "Utility/Util.h"

#include "Kismet/GameplayStatics.h"

Util::Util()
{
}

Util::~Util()
{
}


AActor* Util::GetActorOfClass(const UObject* WorldContextObject,
                              TSubclassOf<AActor> ActorClass)
{
	TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsOfClass(WorldContextObject, ActorClass, FoundActors);
	
	if(FoundActors.Num() < 1)
	{
		UE_LOG(LogTemp,Error,TEXT("Actor not exist"));
		return nullptr;
	}

	return FoundActors[0];
}

TArray<AActor*> Util::GetActorsOfClass(const UObject* WorldContextObject, TSubclassOf<AActor> ActorClass)
{
	TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsOfClass(WorldContextObject, ActorClass, FoundActors);
	
	return FoundActors;
}
