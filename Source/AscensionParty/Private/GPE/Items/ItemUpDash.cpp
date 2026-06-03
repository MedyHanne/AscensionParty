// Fill out your copyright notice in the Description page of Project Settings.


#include "GPE/Items/ItemUpDash.h"
#include <Player/PlayerCharacter.h>
#include <Player/Component/PlayerMovementComponent.h>

// Sets default values
AItemUpDash::AItemUpDash()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	shield = CreateDefaultSubobject<UStaticMeshComponent>("Shield");

	shield->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void AItemUpDash::BeginPlay()
{
	Super::BeginPlay();

	
}

// Called every frame
void AItemUpDash::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AItemUpDash::Use()
{
	if (!refUser||!refUser->GetMovement())return;
	
	
	Server_ReplicateUseUpDash(this, refUser);
}

void AItemUpDash::EndDash(AItemUpDash* _item, APlayerCharacter* _player)
{
	Multicast_ReplicateUseUpDash(_item, _player);
	FTimerHandle _timer;
	GetWorld()->GetTimerManager().SetTimer(_timer, [this, _item, _player]() {

		EndFloating(_item, _player);

		}, floatingTime, false);
}

void AItemUpDash::EndFloating(AItemUpDash* _item, APlayerCharacter* _player)
{
	Multicast_ReplicateEndFloating(_item, _player);
}

void AItemUpDash::Multicast_ReplicateEndFloating_Implementation(AItemUpDash* _item, APlayerCharacter* _player)
{
	UPlayerMovementComponent* _movement = _player->GetMovement();
	_movement->GravityScale = 3;
	_item->shield->SetVisibility(false);
	_item->Super::Use();
}

void AItemUpDash::Multicast_ReplicateEndDash_Implementation(AItemUpDash* _item, APlayerCharacter* _player)
{
	UPlayerMovementComponent* _movement = _player->GetMovement();
	_movement->StopMovementImmediately();
	_movement->GravityScale = 0.5;
}

void AItemUpDash::Server_ReplicateUseUpDash_Implementation(AItemUpDash* _item, APlayerCharacter* _player)
{
	
	FTimerHandle _timer;

	GetWorld()->GetTimerManager().SetTimer(_timer, [this,_item,_player]() {

		EndDash(_item, _player);

	}, dashTime, false);
	Multicast_ReplicateUseUpDash(_item, _player);
}

void AItemUpDash::Multicast_ReplicateUseUpDash_Implementation(AItemUpDash* _item, APlayerCharacter* _player)
{
	UPlayerMovementComponent* _movement = _item->refUser->GetMovement();
	_movement->SetIsDashing(true);
	_item->shield->SetVisibility(true);
	_item->refUser->LaunchCharacter(FVector(0, 0, 4000), true, false);
	_item->shield->AttachToComponent(_player->GetRootComponent(), FAttachmentTransformRules::KeepRelativeTransform);
}


