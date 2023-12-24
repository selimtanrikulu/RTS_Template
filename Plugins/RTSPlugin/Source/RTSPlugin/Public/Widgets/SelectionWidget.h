// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SelectionWidget.generated.h"

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
	
	UPROPERTY(BlueprintReadOnly, meta=(BindWidget),Category="Main Widget")
	UTileView* SelectionTileView;

	UPROPERTY(BlueprintReadOnly, meta=(BindWidget),Category="Main Widget")
	UTextBlock* InfoText;

	UPROPERTY(BlueprintReadOnly, meta=(BindWidget),Category="Main Widget")
	UProgressBar* ProgressBar;
	
private:

	//Dependencies
	UPROPERTY() UStoreManager* StoreManager;
	UPROPERTY() USelectionManager* SelectionManager;

	//Selection Manager Listener
	UFUNCTION() void OnSelectionChanged();

	void CreateSelectionEntries(TArray<URTSEntity*> &SelectedTeamEntities) const;

	void ClearWidget() const;
	
};
