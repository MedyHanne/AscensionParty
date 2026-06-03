// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/OnlinePlayerState.h"
#include <Net/UnrealNetwork.h>
#include <Kismet/KismetSystemLibrary.h>
#include "PlayerCharacter.h"

AOnlinePlayerState::AOnlinePlayerState()
{
	bReplicates = true;
	bAlwaysRelevant = true;
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = true;
}

void AOnlinePlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	//DOREPLIFETIME(AOnlinePlayerState, scorePlayer);
}

void AOnlinePlayerState::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (GetPawn())
	{
		APlayerCharacter* _player = Cast<APlayerCharacter>(GetPawn());
		if (_player)
		{
		FString _text = "player " + FString::FromInt(Cast<APlayerCharacter>(GetPawn())->GetIdController()) + " a score"+ FString::FromInt(scorePlayer);
		UKismetSystemLibrary::PrintString(this, _text);

		}

	}

}

void AOnlinePlayerState::Multicast_ReplicateAddScore_Implementation(AOnlinePlayerState* _player)
{
	_player->scorePlayer++;
	FString _text = "player " + FString::FromInt(Cast<APlayerCharacter>(_player->GetPawn())->GetIdController()) + " a score";
	UKismetSystemLibrary::PrintString(_player, _text);
}
