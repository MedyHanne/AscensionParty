// Fill out your copyright notice in the Description page of Project Settings.


#include "GameMode/OnlineGameMode.h"
#include <PlayerController/OnlinePlayerController.h>
#include <Kismet/KismetSystemLibrary.h>
#include <GameState/OnlineGameState.h>
#include <Player/PlayerCharacter.h>
#include <Kismet/GameplayStatics.h>
#include <Player/OnlinePlayerState.h>

AOnlineGameMode::AOnlineGameMode()
{
	GameStateClass = AOnlineGameState::StaticClass();
	PlayerStateClass = AOnlinePlayerState::StaticClass();
  

    
}

void AOnlineGameMode::BeginPlay()
{
    Super::BeginPlay();

}

void AOnlineGameMode::PostLogin(APlayerController* _newPlayer)
{
	Super::PostLogin(_newPlayer);
    FString Prefix = HasAuthority() ? "[SERVER]" : "[CLIENT]";

    UKismetSystemLibrary::PrintString(this, Prefix+"PostLogin");


	FTimerDelegate _delegate;

	_delegate.BindUObject(this, &AOnlineGameMode::NotifyAllPlayers, _newPlayer);
	GetWorld()->GetTimerManager().SetTimer(timerNotify, _delegate, 1.0f, false);

}




void AOnlineGameMode::NotifyAllPlayers(APlayerController* _newPlayer)
{

    UKismetSystemLibrary::PrintString(this, "NotifyAllPlayers");


    AOnlineGameState* _gs = GetGameState<AOnlineGameState>();
    if (!_gs)
    {
        UKismetSystemLibrary::PrintString(this, "GameState NULL");

        return;
    }
    if (!_newPlayer)
    {
        UKismetSystemLibrary::PrintString(this, "NewPlayer NULL");

        return;
    }
    FString _msg = FString::Printf(TEXT("Multicast envoyé pour : %s"), *_newPlayer->GetName());
    UKismetSystemLibrary::PrintString(this, _msg);

    _gs->Server_NotifyJoinMessage(_newPlayer->GetName());
}

void AOnlineGameMode::HandleSeamlessTravelPlayer(AController*& _c)
{
    Super::HandleSeamlessTravelPlayer(_c);
    onHandleSeamlessTravelPlayer.Broadcast(_c);
}

void AOnlineGameMode::AddPlayer(TObjectPtr<APlayerCharacter> _player)
{
    if (_player && !players.Contains(_player))
    {
        players.Add(_player);
        //UKismetSystemLibrary::PrintString(this, "Add");

    }
}

void AOnlineGameMode::RemovePlayer(TObjectPtr<APlayerCharacter> _player)
{
    //UKismetSystemLibrary::PrintString(this, "Remove");

    if (_player && players.Contains(_player))
    {
        players.Remove(_player);
    }
}

void AOnlineGameMode::CheckPlayer()
{
  /*  UKismetSystemLibrary::PrintString(this, FString::FromInt(players.Num()));
    UKismetSystemLibrary::PrintString(this, "Check");*/
    if (finish)return;
    if (players.Num() == 1)
    {
        OnFinishLevel(players[0]);
    }
}

void AOnlineGameMode::OnFinishLevel(TObjectPtr<APlayerCharacter> _player)
{
    TArray<AActor*> _actors;
    Cast<AOnlinePlayerState>(_player->GetPlayerState())->Multicast_ReplicateAddScore(Cast<AOnlinePlayerState>(_player->GetPlayerState()));
    UGameplayStatics::GetAllActorsOfClass(GetWorld(), APlayerCharacter::StaticClass(), _actors);
    for (AActor* _actor : _actors)
    {
        if (APlayerCharacter* _player = Cast<APlayerCharacter>(_actor))
        {
            _player->Multicast_ReplicateEnableInput(_player, false);
            _player->Multicast_ReplicateShowScore(_player);
        }
    }
    UKismetSystemLibrary::PrintString(this, "Score");
    finish = true;
    ShowScore();
    FTimerHandle _timer;
    GetWorld()->GetTimerManager().SetTimer(_timer, this, &AOnlineGameMode::ChangeLevel, 1.5f, false);
}

void AOnlineGameMode::Multicast_ReplicateEnableInput_Implementation(APlayerController* _player, bool _enable)
{
    if (!_player)
    {
        UKismetSystemLibrary::PrintString(this, "Pas de Controller");
        return;

    }
    UKismetSystemLibrary::PrintString(this, "Input");
    if (_enable)
    {

        _player->EnableInput(_player);
        UKismetSystemLibrary::PrintString(this, "EnableInput");

    }
    else
    {
        _player->DisableInput(_player);
        UKismetSystemLibrary::PrintString(this, "DisableInput");


    }
}

void AOnlineGameMode::ChangeLevel()
{
    GetWorld()->ServerTravel(maps[0].GetLongPackageName() + "?listen");
    UKismetSystemLibrary::PrintString(this, "Change level");

}