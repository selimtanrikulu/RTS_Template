// Fill out your copyright notice in the Description page of Project Settings.


#include "Managers/RTSGameInstance.h"

#include "Kismet/GameplayStatics.h"
#include "Managers/BuildingManager.h"
#include "Managers/LogManager.h"
#include "Managers/RTSHud.h"
#include "Managers/StoreManager.h"
#include "Managers/USelectionManager.h"

void URTSGameInstance::Init()
{
	Super::Init();

	LogManager = NewObject<ULogManager>();
	SelectionManager = NewObject<USelectionManager>();
	BuildingManager = NewObject<UBuildingManager>();
	StoreManager = NewObject<UStoreManager>();

}


void URTSGameInstance::OnBeginPlay()
{
	UWorld* World = GetWorld();
	APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(),0);
	PlayerController->SetShowMouseCursor(true);
	RTSHUD = Cast<ARTSHUD>(PlayerController->GetHUD());
	
	SelectionManager->Begin(World,SelectBoxBP);
	BuildingManager->Begin(World);
	StoreManager->Begin(StoreManagerConfig,StudioBP,World);


}

void URTSGameInstance::OnTick(float DeltaTime)
{
	SelectionManager->Tick(DeltaTime);
	BuildingManager->Tick(DeltaTime);
}



