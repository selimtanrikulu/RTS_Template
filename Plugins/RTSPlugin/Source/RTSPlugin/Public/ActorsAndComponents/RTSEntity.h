// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Utility/Util.h"
#include "RTSEntity.generated.h"

class UEntityManager;
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FEntityAction, URTSEntity*, Entity);


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class RTSPLUGIN_API URTSEntity : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	URTSEntity();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	//Components
	UPROPERTY() UMeshComponent* MeshComponent;

	FEntityAction OnEntityKilledDelegate;

	FVector BoxExtent;
	float Extent;

	
	virtual FEntityData GetEntityData() const;
	virtual FString GetInfo() const;
	virtual float GetProgress() const;

private:

};
