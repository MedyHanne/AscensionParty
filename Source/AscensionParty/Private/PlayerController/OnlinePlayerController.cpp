// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerController/OnlinePlayerController.h"
#include <Kismet/KismetSystemLibrary.h>

void AOnlinePlayerController::Multicast_ShowJoinMessage_Implementation(APlayerController* _newPlayer)
{
	if (this == _newPlayer)return;
	FString _name = _newPlayer ? _newPlayer->GetName() : "Unknow";

	UKismetSystemLibrary::PrintString(this, _name);
}