// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/HUD/GameHUD.h"
#include "Blueprint/UserWidget.h"

void AGameHUD::HideWidget(bool _hide)
{
	if (_hide)
	{
		widgetToHide->SetVisibility(ESlateVisibility::Hidden);
		widgetToHide->SetIsFocusable(false);
	}
	else
	{
		widgetToHide->SetVisibility(ESlateVisibility::Visible);

	}
}


