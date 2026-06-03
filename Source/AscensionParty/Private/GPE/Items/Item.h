// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Item.generated.h"

class APlayerCharacter;
class ASpawnItem;
UCLASS()
class AItem : public AActor
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere)TObjectPtr<UStaticMeshComponent> body;
	UPROPERTY(EditAnywhere)TObjectPtr<UStaticMeshComponent> plane;
	UPROPERTY(EditAnywhere)int indexTexture=0;
	UPROPERTY(VisibleAnywhere)TObjectPtr<ASpawnItem> spawner;
protected:

	UPROPERTY(VisibleAnywhere)TObjectPtr<APlayerCharacter> refUser;

	
public:
	void SetSpawner(ASpawnItem* _spawner) { spawner = _spawner; }
public:	
	// Sets default values for this actor's properties
	AItem();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION() void OnHit(UPrimitiveComponent* OverlappedComp,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepHitResult);
	UFUNCTION(Server, Reliable)
	void Server_ReplicateOnHit(AItem* _item, APlayerCharacter* _player);

	UFUNCTION(NetMulticast, Reliable)
	void Multicast_ReplicateOnHit(AItem* _item, APlayerCharacter* _player);

	virtual void Use();


	UFUNCTION(NetMulticast, Reliable)
	virtual void Multicast_ReplicateUse(AItem* _item, APlayerCharacter* _player);
	
	UFUNCTION(Server, Reliable)
	void Server_ReplicateUse(AItem* _item, APlayerCharacter* _player);

};
