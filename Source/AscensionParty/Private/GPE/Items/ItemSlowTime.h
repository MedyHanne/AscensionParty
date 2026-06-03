// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Item.h"
#include "ItemSlowTime.generated.h"

UCLASS()
class AItemSlowTime : public AItem
{
	GENERATED_BODY()
	UPROPERTY()FTimerHandle timer;
	UPROPERTY()FTimerHandle endTimer;
	UPROPERTY(EditAnywhere)float duration = 5.f;
	UPROPERTY(EditAnywhere)float slowDilation = 0.75f;
	UPROPERTY(EditAnywhere)float speedDilation = 1.75f;
	UPROPERTY(EditAnywhere)TObjectPtr<UMaterialInstance> auraMat;
	UPROPERTY(EditAnywhere)TObjectPtr<UMaterialInstance> previousMat;
	

	bool use = false;

public:	
	// Sets default values for this actor's properties
	AItemSlowTime();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	virtual void Use()override;

	UFUNCTION(Server, Reliable)
	void Server_ReplicateUseSlowTime(AItemSlowTime* _item, APlayerCharacter* _player);

	UFUNCTION(NetMulticast, Reliable)
	void Multicast_ReplicateUseSlowTime(AItemSlowTime* _item, APlayerCharacter* _player);

	UFUNCTION(Server, Reliable)
	void Server_ReplicateUpdateBar(AItemSlowTime* _item, APlayerCharacter* _player, float _timeRemaining);

	UFUNCTION(NetMulticast, Reliable)
	void Multicast_ReplicateUpdateBar(AItemSlowTime* _item, APlayerCharacter* _player, float _timeRemaining);

	UFUNCTION(Server, Reliable)
	void Server_ReplicateEndUse(AItemSlowTime* _item, APlayerCharacter* _player);

	UFUNCTION(NetMulticast, Reliable)
	void Multicast_ReplicateEndUse(AItemSlowTime* _item, APlayerCharacter* _player);


	void UpdateBar();
	void EndUse();
};
