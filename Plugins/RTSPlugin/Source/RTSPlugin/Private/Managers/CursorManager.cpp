// Fill out your copyright notice in the Description page of Project Settings.


#include "Managers/CursorManager.h"

#include "ActorsAndComponents/RTSEntity.h"
#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"
#include "Managers/PlayerManager.h"
#include "Managers/RTSGameInstance.h"
#include "Widgets/MouseCursorWidget.h"


UCursorManager::UCursorManager()
{
	static ConstructorHelpers::FClassFinder<UUserWidget> AttackCursorWidgetAsset(TEXT("/RTSPlugin/Blueprints/Widget/WBP_MouseCursor") );
	if(AttackCursorWidgetAsset.Succeeded())
	{
		CursorBP = AttackCursorWidgetAsset.Class;
	}
}

void UCursorManager::Init(URTSGameInstance* gameInstance)
{
	Super::Init(gameInstance);

	PlayerManager = GameInstance->PlayerManager;
}

void UCursorManager::Begin()
{
	Super::Begin();

	
	//Start Cursor Widget
	UUserWidget* Widget = CreateWidget<UUserWidget>( GameInstance->World,CursorBP);
	MouseCursorWidget = Cast<UMouseCursorWidget>(Widget);
	UGameplayStatics::GetPlayerController
	(GameInstance->World,0)->
	SetMouseCursorWidget(EMouseCursor::Default,MouseCursorWidget);



	SetCursor(EMouseCursorType::Default);
}

void UCursorManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);


	const URTSEntity* EntityUnderCursor = PlayerManager->LookForRTSEntity();

	if(EntityUnderCursor && CurrentMouseCursorType != EMouseCursorType::Highlight)
	{
		SetCursor(EMouseCursorType::Highlight);
	}
	else if(!EntityUnderCursor && CurrentMouseCursorType != EMouseCursorType::Default)
	{
		SetCursor(EMouseCursorType::Default);
	}
}

void UCursorManager::SetCursor(EMouseCursorType MouseCursorType)
{
	if(CurrentMouseCursorType == MouseCursorType)
	{
		UE_LOG(LogTemp,Error,TEXT("Mouse cursor already set"));
		return;
	}

	
	CurrentMouseCursorType = MouseCursorType;
	
	UTexture2D* MouseTexture;
	switch (CurrentMouseCursorType)
	{
		case EMouseCursorType::Default:
			MouseTexture = GameInstance->MouseCursors.DefaultCursorImage;
			break;

		case EMouseCursorType::Highlight:
			MouseTexture = GameInstance->MouseCursors.HighlightCursorImage;
			break;


		case EMouseCursorType::Attack:
			MouseTexture = GameInstance->MouseCursors.AttackCursorImage;
			break;

		default:
			UE_LOG(LogTemp,Error,TEXT("Mouse cursor type is not known"));
			MouseTexture = nullptr;
			return;
	}
	
	MouseCursorWidget->SetCursorImage(MouseTexture);
}
