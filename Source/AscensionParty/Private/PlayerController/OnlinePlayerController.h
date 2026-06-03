// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "OnlinePlayerController.generated.h"

/**
 * 
 */
UCLASS()
class AOnlinePlayerController : public APlayerController
{
	GENERATED_BODY()
	

public:
	UFUNCTION(NetMulticast,Reliable)
	void Multicast_ShowJoinMessage(APlayerController* _newPlayer);
};
