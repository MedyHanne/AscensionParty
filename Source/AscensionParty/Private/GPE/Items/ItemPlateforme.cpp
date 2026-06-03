// Fill out your copyright notice in the Description page of Project Settings.


#include "GPE/Items/ItemPlateforme.h"
#include<Player/PlayerCharacter.h>

// Sets default values
AItemPlateforme::AItemPlateforme()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AItemPlateforme::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AItemPlateforme::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AItemPlateforme::Use()
{
	FVector _spawn = refUser->GetActorLocation();

	_spawn.Z -= 150;
	Server_ReplicateSpawn(this, _spawn);

	Super::Use();

}

void AItemPlateforme::Server_ReplicateSpawn_Implementation(AItemPlateforme* _item,  FVector _spawn)
{
	AActor* _plateform = _item->GetWorld()->SpawnActor<AActor>(_item->plateform, _spawn, _item->GetActorRotation());

}

