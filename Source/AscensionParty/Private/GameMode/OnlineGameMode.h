// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "OnlineGameMode.generated.h"

/**
 * 
 */
class APlayerCharacter;
UCLASS()
class AOnlineGameMode : public AGameMode
{
	GENERATED_BODY()
protected:
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnHandleSeamlessTravelPlayer, AController*, _c);
	UPROPERTY(BlueprintAssignable)FOnHandleSeamlessTravelPlayer onHandleSeamlessTravelPlayer;
	FTimerHandle timerNotify;
	FTimerHandle checkPlayerTimer;
	UPROPERTY()TArray<TObjectPtr<APlayerCharacter>>players;
	bool finish = false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<TSoftObjectPtr<UWorld>> maps;

	
public:
	AOnlineGameMode();
	
protected:
	virtual void BeginPlay()override;
	virtual void PostLogin(APlayerController* _newPlayer)override;

	void NotifyAllPlayers(APlayerController* _newPlayer);

	virtual void HandleSeamlessTravelPlayer(AController*& _c)override;

public:
	void AddPlayer(TObjectPtr<APlayerCharacter> _player);
	void RemovePlayer(TObjectPtr<APlayerCharacter> _player);
	UFUNCTION(BlueprintCallable) void CheckPlayer();
	UFUNCTION(NetMulticast, Reliable,BlueprintCallable)
	void Multicast_ReplicateEnableInput(APlayerController* _player, bool _enable);
	void OnFinishLevel(TObjectPtr<APlayerCharacter> _player);
	UFUNCTION(BlueprintImplementableEvent)void ShowScore();
	void ChangeLevel();

};
