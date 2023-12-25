// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/MouseCursorWidget.h"

#include "Components/Image.h"

void UMouseCursorWidget::SetCursorImage(UTexture2D* Texture2D)
{
	CursorImage->SetBrushFromTexture(Texture2D);
}
