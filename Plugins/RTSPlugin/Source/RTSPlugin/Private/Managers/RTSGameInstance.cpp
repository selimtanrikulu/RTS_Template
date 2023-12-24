// Fill out your copyright notice in the Description page of Project Settings.


#include "Managers/RTSGameInstance.h"

#include "Managers/PlayerManager.h"
#include "Kismet/GameplayStatics.h"
#include "Managers/Asset_Manager.h"
#include "Managers/BuildingManager.h"
#include "Managers/LogManager.h"
#include "Managers/RTSHud.h"
#include "Managers/StoreManager.h"
#include "..\..\Public\Managers\SelectionManager.h"
#include "Managers/EntityManager.h"
#include "Managers/SourceManager.h"

void URTSGameInstance::Init()
{
	Super::Init();

	LogManager = NewObject<ULogManager>();
	SelectionManager = NewObject<USelectionManager>();
	BuildingManager = NewObject<UBuildingManager>();
	StoreManager = NewObject<UStoreManager>();
	AssetManager = NewObject<UAsset_Manager>();
	PlayerManager = NewObject<UPlayerManager>();
	EntityManager = NewObject<UEntityManager>();
	SourceManager = NewObject<USourceManager>();

	
	Managers.Add(LogManager);
	Managers.Add(SelectionManager);
	Managers.Add(BuildingManager);
	Managers.Add(StoreManager);
	Managers.Add(AssetManager);
	Managers.Add(PlayerManager);
	Managers.Add(EntityManager);
	Managers.Add(SourceManager);

	for(UManagerBase* ManagerBase : Managers)
	{
		ManagerBase->Init(this);
	}
	for(UManagerBase* ManagerBase : Managers)
	{
		ManagerBase->PostInit();
	}
}


void URTSGameInstance::OnBeginPlay()
{
	World = GetWorld();
	APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(),0);
	PlayerController->SetShowMouseCursor(true);
	RTSHUD = Cast<ARTSHUD>(PlayerController->GetHUD());


	for(UManagerBase* ManagerBase : Managers)
	{
		ManagerBase->Begin();
	}
}

void URTSGameInstance::OnTick(float DeltaTime)
{
	for(UManagerBase* ManagerBase : Managers)
	{
		ManagerBase->Tick(DeltaTime);
	}
}



