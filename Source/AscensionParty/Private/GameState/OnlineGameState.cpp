// Fill out your copyright notice in the Description page of Project Settings.


#include "GameState/OnlineGameState.h"
#include <Kismet/KismetSystemLibrary.h>

AOnlineGameState::AOnlineGameState()
{
	bReplicates = true;
	bAlwaysRelevant = true;

}

void AOnlineGameState::Server_NotifyJoinMessage_Implementation(const FString& _playerName)
{
	//Multicast_ShowJoinMessage(_playerName);
}

void AOnlineGameState::Multicast_ShowJoinMessage_Implementation(const FString& _playerName)
{
	FString Mode;

	switch (GetWorld()->GetNetMode())
	{
	case NM_Client: Mode = "CLIENT"; break;
	case NM_ListenServer: Mode = "LISTEN SERVER"; break;
	case NM_DedicatedServer: Mode = "DEDICATED SERVER"; break;
	default: Mode = "STANDALONE"; break;
	}

	UKismetSystemLibrary::PrintString(this, Mode, true, true);

}
