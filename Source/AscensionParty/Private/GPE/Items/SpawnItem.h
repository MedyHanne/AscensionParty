// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SpawnItem.generated.h"

class AItem;
UCLASS()
class ASpawnItem : public AActor
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere)TArray<TSubclassOf<AItem>> items;
	UPROPERTY(EditAnywhere)float timeToSpawn=3.0f;
	
public:	
	// Sets default values for this actor's properties
	ASpawnItem();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	void Spawn();
	void OnRespawn();
	UFUNCTION(Server, Reliable)
	void Server_ReplicateOnRespawn(ASpawnItem* _item);

};
