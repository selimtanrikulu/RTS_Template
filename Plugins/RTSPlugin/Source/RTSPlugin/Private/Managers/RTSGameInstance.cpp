// Fill out your copyright notice in the Description page of Project Settings.


#include "Managers/RTSGameInstance.h"

#include "Kismet/GameplayStatics.h"
#include "Managers/LogManager.h"
#include "Managers/USelectionManager.h"

void URTSGameInstance::Init()
{
	Super::Init();

	LogManager = NewObject<ULogManager>();
	SelectionManager = NewObject<USelectionManager>();
}

void URTSGameInstance::OnBeginPlay()
{
	UWorld* World = GetWorld();
	
	APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(),0);
	PlayerController->SetShowMouseCursor(true);

	SelectionManager->Begin(World,SelectBoxBP);
	
	
}

void URTSGameInstance::OnTick(float DeltaTime)
{
	SelectionManager->Tick(DeltaTime);	
}



