// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TeamEntity.generated.h"

UCLASS()
class RTSPLUGIN_API ATeamEntity : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATeamEntity();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;


	
	//Components
	UPROPERTY() UMeshComponent* MeshComponent;
	FVector BoxExtent;
	float Extent;
	
};
