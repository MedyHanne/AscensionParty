// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Item.h"
#include "ItemUpDash.generated.h"

UCLASS()
class AItemUpDash : public AItem
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere)TObjectPtr<UStaticMeshComponent>shield;
	UPROPERTY(EditAnywhere)float dashTime=5;
	UPROPERTY(EditAnywhere)float floatingTime=0.5;
public:	
	// Sets default values for this actor's properties
	AItemUpDash();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	virtual void Use()override;
	void EndDash(AItemUpDash* _item, APlayerCharacter* _player);
	void EndFloating(AItemUpDash* _item, APlayerCharacter* _player);

	UFUNCTION(Server, Reliable)
	void Server_ReplicateUseUpDash(AItemUpDash* _item, APlayerCharacter* _player);

	UFUNCTION(NetMulticast, Reliable)
	void Multicast_ReplicateUseUpDash(AItemUpDash* _item, APlayerCharacter* _player);
	UFUNCTION(NetMulticast, Reliable)
	void Multicast_ReplicateEndDash(AItemUpDash* _item, APlayerCharacter* _player);
	UFUNCTION(NetMulticast, Reliable)
	void Multicast_ReplicateEndFloating(AItemUpDash* _item, APlayerCharacter* _player);
};
