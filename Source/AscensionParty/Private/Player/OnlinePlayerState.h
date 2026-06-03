// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "OnlinePlayerState.generated.h"

/**
 * 
 */
UCLASS()
class AOnlinePlayerState : public APlayerState
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadOnly)int scorePlayer = 0;
	FORCEINLINE void  AddScore() { scorePlayer++; };
	FORCEINLINE int  GetScorePlayer() { return scorePlayer; };

public:
	AOnlinePlayerState();

	virtual void GetLifetimeReplicatedProps(
		TArray<FLifetimeProperty>& OutLifetimeProps) const override;

protected:
	virtual void Tick(float DeltaTime) override;
public:
	UFUNCTION(NetMulticast, Reliable, BlueprintCallable)
	void Multicast_ReplicateAddScore(AOnlinePlayerState* _player);
};
