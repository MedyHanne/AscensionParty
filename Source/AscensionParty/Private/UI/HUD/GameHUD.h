// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "GameHUD.generated.h"

/**
 * 
 */
UCLASS()
class AGameHUD : public AHUD
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite)TObjectPtr<UUserWidget> widgetToHide;

	void HideWidget(bool _hide);

	UFUNCTION(BlueprintImplementableEvent)
	void UpdateCountdownTest(int32  _count);
	UFUNCTION(BlueprintImplementableEvent)void ShowScore();
	UFUNCTION(BlueprintImplementableEvent)void EndCountDown();
};
