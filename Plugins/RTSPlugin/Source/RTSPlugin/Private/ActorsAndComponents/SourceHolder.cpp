// Fill out your copyright notice in the Description page of Project Settings.


#include "ActorsAndComponents/SourceHolder.h"

#include "ActorsAndComponents/NeutralEntity.h"
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
	
	NeutralEntity = CreateDefaultSubobject<UNeutralEntity>(TEXT("Neutral Entity"));
}

void USourceHolder::Init(const FSourceData& sourceData)
{
	SourceData = sourceData;
	AmountLeft = SourceData.HoldAmount;

	//NeutralEntity->Init()
}


// Called when the game starts
void USourceHolder::BeginPlay()
{
	Super::BeginPlay();

	// ...

	const APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(),0);
	const URTSGameInstance* GameInstance = Cast<URTSGameInstance>(PlayerController->GetGameInstance());
	SourceManager = GameInstance->SourceManager;
}


// Called every frame
void USourceHolder::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

FSourceData USourceHolder::GetSourceData() const
{
	return SourceData;
}

void USourceHolder::Collect()
{
	AmountLeft --;
	SourceManager->Earn(SourceData.EntityData.Name,1);
}

float USourceHolder::GetProgress() const
{
	return static_cast<float>(AmountLeft) / SourceData.HoldAmount;
}

FString USourceHolder::GetInfo() const
{
	return SourceData.GetInfo();
}

