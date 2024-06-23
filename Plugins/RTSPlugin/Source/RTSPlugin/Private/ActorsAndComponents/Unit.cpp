// Fill out your copyright notice in the Description page of Project Settings.


#include "ActorsAndComponents/Unit.h"

#include "AIController.h"
#include "ActorsAndComponents/TeamEntity.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Managers/Asset_Manager.h"
#include "Managers/RTSGameInstance.h"


// Sets default values
AUnit::AUnit()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;


	TeamEntity = CreateDefaultSubobject<UTeamEntity>(TEXT("Team Entity"));
	

	AutoPossessAI = EAutoPossessAI::Disabled;


	bUseControllerRotationYaw = false;
	GetCharacterMovement()->bOrientRotationToMovement = true;
}

void AUnit::Init(const FUnitData& unitData)
{
	UnitData = unitData;
	TeamEntity->Init(UnitData.TeamEntityData);
	TeamEntity->HealFull();
}

// Called when the game starts or when spawned
void AUnit::BeginPlay()
{
	Super::BeginPlay();

	const APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(),0);
	const URTSGameInstance* GameInstance = Cast<URTSGameInstance>(PlayerController->GetGameInstance());

	AssetManager = GameInstance->AssetManager;
	
	AIControllerClass = AssetManager->GetAIControllerBP();
	AActor* SpawnedActor = GetWorld()->SpawnActor<AActor>(AIControllerClass,FVector(),FRotator());
	AAIController* SpawnedAIController = Cast<AAIController>(SpawnedActor);
	SpawnedAIController->Possess(this);

	BlackboardComponent = SpawnedAIController->GetBlackboardComponent();

	
}

// Called every frame
void AUnit::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);


	UpdateUnitAnimation();
}

void AUnit::SetTargetRTSEntity(const URTSEntity* Entity)
{
	const FVector TargetLocation = Entity->GetOwner()->GetActorLocation();
	BlackboardComponent->SetValueAsVector("TargetLocation",TargetLocation);
}

void AUnit::SetTargetLocation(const FVector& TargetLocation)
{
	BlackboardComponent->SetValueAsVector("TargetLocation",TargetLocation);
}

FUnitData AUnit::GetUnitData() const
{
	return UnitData;
}

void AUnit::UpdateUnitAnimation()
{
	UnitAnimationState = EUnitAnimationState::Idle;	
}


