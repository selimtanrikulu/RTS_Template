// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Utility/Util.h"
#include "Source.generated.h"

class USourceHolder;
class UNeutralEntity;

UCLASS()
class RTSPLUGIN_API ASource : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASource();
	void Init(const FEntityData &EntityData);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	
	//Components
	UPROPERTY(EditDefaultsOnly) UStaticMeshComponent* StaticMeshComponent;
	UPROPERTY(EditDefaultsOnly) UNeutralEntity* NeutralEntity;
	UPROPERTY(EditDefaultsOnly) USourceHolder* SourceHolder;


	
	
};
