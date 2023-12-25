// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MouseCursorWidget.generated.h"

class UImage;
/**
 * 
 */
UCLASS()
class RTSPLUGIN_API UMouseCursorWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	

	UPROPERTY(BlueprintReadOnly, meta=(BindWidget),Category="Building Widget")
	UImage* CursorImage;


	void SetCursorImage(UTexture2D* Texture2D);
	
	
	
};
