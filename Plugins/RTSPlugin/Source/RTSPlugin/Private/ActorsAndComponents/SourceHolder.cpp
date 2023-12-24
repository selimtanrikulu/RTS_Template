// Fill out your copyright notice in the Description page of Project Settings.


#include "ActorsAndComponents/SourceHolder.h"

#include "ActorsAndComponents/Building.h"
#include "ActorsAndComponents/NeutralEntity.h"
#include "ActorsAndComponents/Source.h"
#include "Kismet/GameplayStatics.h"
#include "Managers/RTSGameInstance.h"
#include "Managers/SourceManager.h"

// Sets default values for this component's properties
USourceHolder::USourceHolder()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}




// Called when the game starts
void USourceHolder::BeginPlay()
{
	Super::BeginPlay();
	
	// ...

	const APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(),0);
	const URTSGameInstance* GameInstance = Cast<URTSGameInstance>(PlayerController->GetGameInstance());
	SourceManager = GameInstance->SourceManager;

	AmountLeft = HoldAmount;
}


// Called every frame
void USourceHolder::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);


	if(AmountLeft > 0)
	{
		//Collect();
	}


	

	
	// ...
}


void USourceHolder::Collect()
{
	AmountLeft --;
	SourceManager->Earn(SourceName,1);

	OnSourceCollectedDelegate.Broadcast(this);

	if(AmountLeft <= 0)
	{
		OnSourceFinishedDelegate.Broadcast(this);
	}
}

float USourceHolder::GetProgress() const
{
	return static_cast<float>(AmountLeft) / HoldAmount;
}

FString USourceHolder::GetInfo() const
{
	AActor* Owner = GetOwner();


	if(const ASource* Source = Cast<ASource>(Owner))
	{
		return Source->NeutralEntity->GetInfo();
	}
	if(const ABuilding* Building = Cast<ABuilding>(Owner))
	{
		return Building->GetBuildingData().GetInfo();
	}

	UE_LOG(LogTemp,Error,TEXT("Info not found for source holder"));
	
	return "";
}

