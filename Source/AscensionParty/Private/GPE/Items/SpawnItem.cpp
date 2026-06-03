// Fill out your copyright notice in the Description page of Project Settings.


#include "GPE/Items/SpawnItem.h"
#include <Kismet/KismetMathLibrary.h>
#include "Item.h"

// Sets default values
ASpawnItem::ASpawnItem()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	RootComponent = CreateDefaultSubobject<USceneComponent>("Root");

}

// Called when the game starts or when spawned
void ASpawnItem::BeginPlay()
{
	Super::BeginPlay();
	if (GetNetMode() == NM_DedicatedServer || GetNetMode() == NM_ListenServer)
		Spawn();

}

// Called every frame
void ASpawnItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ASpawnItem::Spawn()
{
	int _index = UKismetMathLibrary::RandomIntegerInRange(0, items.Num()-1);
	AItem* _item = GetWorld()->SpawnActor<AItem>(items[_index],GetActorLocation(),GetActorRotation());
	if(_item)
		_item->SetSpawner(this);
}
void ASpawnItem::OnRespawn()
{
	if (GetNetMode() == NM_DedicatedServer || GetNetMode() == NM_ListenServer)
	{
	FTimerHandle _timer;
	GetWorld()->GetTimerManager().SetTimer(_timer, this, &ASpawnItem::Spawn, timeToSpawn, false);
	}
	else
	{
		Server_ReplicateOnRespawn(this);
	}
}

void ASpawnItem::Server_ReplicateOnRespawn_Implementation(ASpawnItem* _item)
{
	FTimerHandle _timer;
	_item->GetWorld()->GetTimerManager().SetTimer(_timer, _item, &ASpawnItem::Spawn, _item->timeToSpawn, false);
}

