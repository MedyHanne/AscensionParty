// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Item.h"
#include "ItemDoubleJump.generated.h"

UCLASS()
class AItemDoubleJump : public AItem
{
	GENERATED_BODY()
	UPROPERTY()FTimerHandle timer;
	UPROPERTY()FTimerHandle endTimer;
	UPROPERTY(EditAnywhere)float duration=5.f;
	UPROPERTY(EditAnywhere)TObjectPtr<UMaterialInstance> auraMat;
	UPROPERTY(EditAnywhere)TObjectPtr<UMaterialInstance> previousMat;

	bool use = false;
	
public:	
	// Sets default values for this actor's properties
	AItemDoubleJump();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void Use()override;

	UFUNCTION(Server, Reliable)
	void Server_ReplicateUseJump(AItemDoubleJump* _item, APlayerCharacter* _player);

	UFUNCTION(NetMulticast, Reliable)
	void Multicast_ReplicateUseJump(AItemDoubleJump* _item, APlayerCharacter* _player);

	UFUNCTION(Server, Reliable)
	void Server_ReplicateUpdateBar(AItemDoubleJump* _item, APlayerCharacter* _player,float _timeRemaining);

	UFUNCTION(NetMulticast, Reliable)
	void Multicast_ReplicateUpdateBar(AItemDoubleJump* _item, APlayerCharacter* _player, float _timeRemaining);
	
	UFUNCTION(Server, Reliable)
	void Server_ReplicateEndUse(AItemDoubleJump* _item, APlayerCharacter* _player);

	UFUNCTION(NetMulticast, Reliable)
	void Multicast_ReplicateEndUse(AItemDoubleJump* _item, APlayerCharacter* _player);

	
	void UpdateBar();
	void EndUse();

};
