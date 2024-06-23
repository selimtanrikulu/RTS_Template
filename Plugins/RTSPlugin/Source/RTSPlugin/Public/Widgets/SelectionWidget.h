// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SelectionWidget.generated.h"

class UImage;
class URTSEntity;
class UTextBlock;
class UTeamEntity;
class USelectionManager;
class UStoreManager;
class UProgressBar;
class UTileView;
/**
 * 
 */
UCLASS()
class USelectionWidget : public UUserWidget
{
	GENERATED_BODY()

public:

	virtual void NativeConstruct() override;
	
	UPROPERTY(BlueprintReadOnly, meta=(BindWidget),Category="Selection Widget")
	UImage* PanelBackground;
	
	UPROPERTY(BlueprintReadOnly, meta=(BindWidget),Category="Selection Widget")
	UTileView* SelectionTileView;

	UPROPERTY(BlueprintReadOnly, meta=(BindWidget),Category="Selection Widget")
	UTextBlock* InfoText;

	UPROPERTY(BlueprintReadOnly, meta=(BindWidget),Category="Selection Widget")
	UProgressBar* ProgressBar;
	
private:

	//Dependencies
	UPROPERTY() UStoreManager* StoreManager;
	UPROPERTY() USelectionManager* SelectionManager;

	//Selection Manager Listener
	UFUNCTION() void OnSelectedEntitiesChanged();
	UFUNCTION() void OnTeamEntityHPChangedDelegate(UTeamEntity* TeamEntity);



	void UpdateProgressBar() const;
	void UpdateInfoText() const;
	void UpdateSelectionEntries() const;
	
	
};
