// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Source.generated.h"

class UTeamEntity;

UCLASS()
class RTSPLUGIN_API ASource : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASource();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY() UTeamEntity* TeamEntity;
	
};
