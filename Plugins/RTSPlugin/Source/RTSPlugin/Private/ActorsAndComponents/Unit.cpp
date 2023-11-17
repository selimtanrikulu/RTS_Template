// Fill out your copyright notice in the Description page of Project Settings.


#include "ActorsAndComponents/Unit.h"

#include "AIController.h"
#include "ActorsAndComponents/TeamEntity.h"
#include "BehaviorTree/BlackboardComponent.h"
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
	


	UCharacterMovementComponent* CharacterMovementComponent = GetCharacterMovement();
	//CharacterMovementComponent->SetAvoidanceEnabled(true);
	//CharacterMovementComponent->AvoidanceConsiderationRadius = 500;
}

// Called every frame
void AUnit::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AUnit::SetTargetLocation(FVector TargetLocation)
{

	AAIController* AIController = Cast<AAIController>(GetController());
	UBlackboardComponent* BlackboardComponent = AIController->GetBlackboardComponent();

	BlackboardComponent->SetValueAsVector("TargetLocation",TargetLocation);
}

