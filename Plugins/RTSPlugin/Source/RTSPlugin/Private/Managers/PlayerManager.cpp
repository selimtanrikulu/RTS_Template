// Fill out your copyright notice in the Description page of Project Settings.


#include "Managers/PlayerManager.h"

#include "Managers/RTSPawn.h"

void UPlayerManager::Init(URTSGameInstance* gameInstance)
{
	Super::Init(gameInstance);
}

void UPlayerManager::Begin()
{
	Super::Begin();
}

void UPlayerManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void UPlayerManager::SetPawn(ARTSPawn* rTSPawn)
{
	RTSPawn = rTSPawn;
	
	RTSPawn->OnMouseLeftClickDelegate.AddUniqueDynamic(this,&UPlayerManager::OnMouseLeftClicked);
	RTSPawn->OnMouseLeftReleasedDelegate.AddUniqueDynamic(this,&UPlayerManager::OnMouseLeftReleased);
	RTSPawn->OnMouseRightClickDelegate.AddUniqueDynamic(this,&UPlayerManager::OnMouseRightClicked);
	RTSPawn->OnMouseRightReleasedDelegate.AddUniqueDynamic(this,&UPlayerManager::OnMouseRightReleased);
	RTSPawn->OnMouseWheelDownDelegate.AddUniqueDynamic(this,&UPlayerManager::OnMouseWheelDown);
	RTSPawn->OnMouseWheelUpDelegate.AddUniqueDynamic(this,&UPlayerManager::OnMouseWheelUp);
	RTSPawn->OnDeleteClickDelegate.AddUniqueDynamic(this,&UPlayerManager::OnDeleteClicked);
}

FHitResult UPlayerManager::LookForFloor() const
{
	if(RTSPawn)
	{
		return RTSPawn->LookForFloor();
	}

	UE_LOG(LogTemp, Error, TEXT("Pawn is not set for look for floor"));
	FHitResult Empty;
	return Empty;
}

URTSEntity* UPlayerManager::LookForRTSEntity() const
{
	if(RTSPawn)
	{
		return RTSPawn->LookForRTSEntity();
	}

	return nullptr;
}

void UPlayerManager::OnMouseLeftClicked()
{
	OnMouseLeftClickDelegate.Broadcast();
}

void UPlayerManager::OnMouseLeftReleased()
{
	OnMouseLeftReleasedDelegate.Broadcast();
}

void UPlayerManager::OnMouseRightClicked()
{
	OnMouseRightClickDelegate.Broadcast();
}

void UPlayerManager::OnMouseRightReleased()
{
	OnMouseRightReleasedDelegate.Broadcast();
}

void UPlayerManager::OnMouseWheelUp()
{
	OnMouseWheelUpDelegate.Broadcast();
}

void UPlayerManager::OnMouseWheelDown()
{
	OnMouseWheelDownDelegate.Broadcast();
}

void UPlayerManager::OnDeleteClicked()
{
	OnDeleteClickDelegate.Broadcast();
}
