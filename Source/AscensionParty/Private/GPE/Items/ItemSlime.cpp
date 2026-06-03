// Fill out your copyright notice in the Description page of Project Settings.


#include "GPE/Items/ItemSlime.h"
#include "SlimeProjectile.h"
#include<Player/PlayerCharacter.h>

// Sets default values
AItemSlime::AItemSlime()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AItemSlime::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AItemSlime::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AItemSlime::Use()
{
	Server_ReplicateSpawn(this);

	Super::Use();
}

void AItemSlime::Server_ReplicateSpawn_Implementation(AItemSlime* _item)
{
    if (!_item || !_item->refUser) return;

    // Spawn devant le joueur, pas SUR le joueur
    FVector spawnLocation = _item->refUser->GetActorLocation()
        + _item->refUser->GetActorForwardVector() * 100.f
        + FVector(0, 0, 50.f); // légère hauteur

    ASlimeProjectile* _projectile = _item->GetWorld()->SpawnActor<ASlimeProjectile>(
        _item->slime,
        spawnLocation,        // ← plus sur le joueur
        FRotator(-90, 0, 0)
    );
    _projectile->SetShooterRef(_item->refUser);
}

