// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameState.h"
#include "OnlineGameState.generated.h"

/**
 * 
 */
UCLASS()
class AOnlineGameState : public AGameState
{
	GENERATED_BODY()
	

	

public:
	AOnlineGameState();

	UFUNCTION(NetMulticast, Reliable)
	void Multicast_ShowJoinMessage(const FString& _playerName);
	UFUNCTION(Server,Reliable)
	void Server_NotifyJoinMessage(const FString& _playerName);
};
