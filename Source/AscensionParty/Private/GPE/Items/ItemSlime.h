// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Item.h"

#include "ItemSlime.generated.h"

UCLASS()
class AItemSlime : public AItem
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere)TSubclassOf<AActor>slime;

public:	
	// Sets default values for this actor's properties
	AItemSlime();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	virtual void Use()override;

	UFUNCTION(Server, Reliable)
	void Server_ReplicateSpawn(AItemSlime* _item);

};
