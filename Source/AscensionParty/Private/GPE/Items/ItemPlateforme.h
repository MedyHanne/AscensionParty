// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Item.h"
#include "ItemPlateforme.generated.h"

UCLASS()
class AItemPlateforme : public AItem
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere)TSubclassOf<AActor>plateform;
	
public:	
	// Sets default values for this actor's properties
	AItemPlateforme();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	virtual void Use()override;

	UFUNCTION(Server, Reliable)
	void Server_ReplicateSpawn(AItemPlateforme* _item, FVector _spawn);
};
