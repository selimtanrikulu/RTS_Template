// Fill out your copyright notice in the Description page of Project Settings.


#include "RTSPlugin/Public/Managers/RTSPawn.h"

#include "Managers/PlayerManager.h"
#include "GameFramework/SpringArmComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Managers/BuildingManager.h"
#include "Managers/LogManager.h"
#include "Managers/RTSGameInstance.h"

// Sets default values
ARTSPawn::ARTSPawn()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ARTSPawn::BeginPlay()
{
	Super::BeginPlay();
	
	PlayerController = UGameplayStatics::GetPlayerController(GetWorld(),0);
	const URTSGameInstance* RTSGameInstance = Cast<URTSGameInstance>(PlayerController->GetGameInstance());
	
	LogManager = RTSGameInstance->LogManager;
	BuildingManager = RTSGameInstance->BuildingManager;
	PlayerManager = RTSGameInstance->PlayerManager;

	PlayerManager->SetPawn(this);
	
	SpringArmComponent = FindComponentByClass<USpringArmComponent>();
}

// Called every frame
void ARTSPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	Delta_Time = DeltaTime;

	Move();
}

// Called to bind functionality to input
void ARTSPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	PlayerInputComponent->BindAxis("Horizontal",this,&ARTSPawn::SetHorizontalInput);
	PlayerInputComponent->BindAxis("Vertical",this,&ARTSPawn::SetVerticalInput);
	PlayerInputComponent->BindAction("MouseLeft",IE_Pressed,this,&ARTSPawn::OnMouseLeftClicked);
	PlayerInputComponent->BindAction("MouseLeft",IE_Released,this,&ARTSPawn::OnMouseLeftReleased);
	PlayerInputComponent->BindAction("MouseRight",IE_Pressed,this,&ARTSPawn::OnMouseRightClicked);
	PlayerInputComponent->BindAction("MouseRight",IE_Released,this,&ARTSPawn::OnMouseRightReleased);
	PlayerInputComponent->BindAction("MouseWheelDown",IE_Pressed,this,&ARTSPawn::OnMouseWheelDown);
	PlayerInputComponent->BindAction("MouseWheelUp",IE_Pressed,this,&ARTSPawn::OnMouseWheelUp);
	PlayerInputComponent->BindAction("Delete",IE_Pressed,this,&ARTSPawn::OnDeleteClicked);
}

void ARTSPawn::SetHorizontalInput(float Value)
{
	HorizontalInput = Value;
}

void ARTSPawn::SetVerticalInput(float Value)
{
	VerticalInput = Value;
}

void ARTSPawn::OnMouseLeftClicked()
{
	LogManager->EditorLog(FText::FromString(TEXT("Left Clicked")),ELogVerbosity::Display);
	OnMouseLeftClickDelegate.Broadcast();
}

void ARTSPawn::OnMouseLeftReleased()
{
	LogManager->EditorLog(FText::FromString(TEXT("Left Released")),ELogVerbosity::Display);
	OnMouseLeftReleasedDelegate.Broadcast();
}

void ARTSPawn::OnMouseRightClicked()
{
	LogManager->EditorLog(FText::FromString(TEXT("Right Clicked")),ELogVerbosity::Display);
	OnMouseRightClickDelegate.Broadcast();
}

void ARTSPawn::OnMouseRightReleased()
{
	LogManager->EditorLog(FText::FromString(TEXT("Right Released")),ELogVerbosity::Display);
	OnMouseRightReleasedDelegate.Broadcast();
}

void ARTSPawn::OnMouseWheelUp()
{
	LogManager->EditorLog(FText::FromString(TEXT("Wheel Up")),ELogVerbosity::Display);
	OnMouseWheelUpDelegate.Broadcast();

	UpdateZoom(-1);
}

void ARTSPawn::OnMouseWheelDown()
{
	LogManager->EditorLog(FText::FromString(TEXT("Wheel Down")),ELogVerbosity::Display);
	OnMouseWheelDownDelegate.Broadcast();

	UpdateZoom(1);
}

void ARTSPawn::OnDeleteClicked()
{
	LogManager->EditorLog(FText::FromString(TEXT("Delete Clicked")),ELogVerbosity::Display);
	OnDeleteClickDelegate.Broadcast();
}

void ARTSPawn::Move()
{
	if(FMath::Abs(HorizontalInput) < 0.01f && FMath::Abs(VerticalInput) < 0.01f) return;
	
	FVector Direction(-HorizontalInput,VerticalInput,0);
	Direction.Normalize();

	const FVector Location = GetActorLocation();
	
	SetActorLocation(Location + Direction * MovementSpeed * Delta_Time);
}

void ARTSPawn::UpdateZoom(const int Amount)
{
	if(BuildingManager->IsDrafting())return;
	
	CurrentZoomLevel += Amount;

	CurrentZoomLevel = FMath::Max(0,CurrentZoomLevel);
	CurrentZoomLevel = FMath::Min(MaxZoomLevel,CurrentZoomLevel);
	
	SpringArmComponent->TargetArmLength = CurrentZoomLevel * ZoomInterval;
}



FHitResult ARTSPawn::LookForFloor() const
{
	FHitResult Hit;
	PlayerController->GetHitResultUnderCursor(ECC_GameTraceChannel1,false,Hit);
	return Hit;
}