// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "RTSPawn.generated.h"


class URTSEntity;
class UBuildingManager;
class USpringArmComponent;
class ULogManager;
class UPlayerManager;
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnMyInputAction);


UCLASS()
class RTSPLUGIN_API ARTSPawn : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ARTSPawn();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	//Input Events
	UPROPERTY(BlueprintAssignable) FOnMyInputAction OnMouseLeftClickDelegate;
	UPROPERTY(BlueprintAssignable) FOnMyInputAction OnMouseLeftReleasedDelegate;
	UPROPERTY(BlueprintAssignable) FOnMyInputAction OnMouseRightClickDelegate;
	UPROPERTY(BlueprintAssignable) FOnMyInputAction OnMouseRightReleasedDelegate;
	UPROPERTY(BlueprintAssignable) FOnMyInputAction OnMouseWheelUpDelegate;
	UPROPERTY(BlueprintAssignable) FOnMyInputAction OnMouseWheelDownDelegate;
	UPROPERTY(BlueprintAssignable) FOnMyInputAction OnDeleteClickDelegate;

	
	//LineTraces
	FHitResult LookForFloor() const;
	URTSEntity* LookForRTSEntity() const;

	

private:

	//Dependencies
	UPROPERTY() UPlayerManager* PlayerManager;
	UPROPERTY() ULogManager* LogManager;
	UPROPERTY() UBuildingManager* BuildingManager;
	

	//Inputs
	float HorizontalInput;
	float VerticalInput;
	void SetHorizontalInput(float Value);
	void SetVerticalInput(float Value);

	
	//Input Actions
	void OnMouseLeftClicked();
	void OnMouseLeftReleased();
	void OnMouseRightClicked();
	void OnMouseRightReleased();
	void OnMouseWheelUp();
	void OnMouseWheelDown();
	void OnDeleteClicked();

	
	


	//Components
	UPROPERTY() USpringArmComponent* SpringArmComponent;



	//Movement
	void Move();
	void UpdateZoom(int Amount);
	

	//Utility
	UPROPERTY() APlayerController* PlayerController;
	float Delta_Time;
	int CurrentZoomLevel = 0;


	//Config
	float MovementSpeed = 1000;
	int MaxZoomLevel = 25;
	int ZoomInterval = 40;
	
	
};
