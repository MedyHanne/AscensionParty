// Fill out your copyright notice in the Description page of Project Settings.


#include "GPE/Obstacles/ObstacleIceBlock.h"
#include <Player/PlayerCharacter.h>
#include<Player/Component/PlayerMovementComponent.h>
#include <Kismet/KismetSystemLibrary.h>

// Sets default values
AObstacleIceBlock::AObstacleIceBlock()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AObstacleIceBlock::BeginPlay()
{
	Super::BeginPlay();
	box->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
		UKismetSystemLibrary::PrintString(this, "Init ICE BLOCK",true,true,FLinearColor::Blue,10);
	if (GetNetMode() == NM_DedicatedServer || GetNetMode() == NM_ListenServer)
	{

		box->OnComponentBeginOverlap.AddDynamic(this, &AObstacleIceBlock::OnWalkIn);
		//box->OnComponentEndOverlap.AddDynamic(this, &AObstacleIceBlock::OnWalkOut);
		box->OnComponentEndOverlap.AddDynamic(this, &AObstacleIceBlock::OnWalkOut);

	}
}

// Called every frame
void AObstacleIceBlock::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AObstacleIceBlock::OnWalkIn(UPrimitiveComponent* _overlappedComponent, AActor* _otherActor, UPrimitiveComponent* _otherComp, int32 _otherBodyIndex, bool _bFromSweep, const FHitResult& _sweepResult)
{
	APlayerCharacter* _player = Cast<APlayerCharacter>(_otherActor);
	UKismetSystemLibrary::PrintString(_player, "OnWalkIn");


	if (_player)
	{

		Multicast_ReplicateOnWalkIn(_player, groundFriction,brakingDecelerationWalking);
		

	}
}

void AObstacleIceBlock::OnWalkOut(UPrimitiveComponent* _overlappedComponent, AActor* _otherActor, UPrimitiveComponent* _otherComp, int32 _otherBodyIndex)
{
	APlayerCharacter* _player = Cast<APlayerCharacter>(_otherActor);


	if (_player)
	{

		Multicast_ReplicateOnWalkOut(_player, groundFriction, brakingDecelerationWalking);

	}
}


void AObstacleIceBlock::Multicast_ReplicateOnWalkIn_Implementation(APlayerCharacter* _player, float _groundFriction, float _brakingDecelerationWalking)
{
	UKismetSystemLibrary::PrintString(_player, "Glisse");
	_player->GetCharacterMovement()->GroundFriction = _groundFriction;
	_player->GetCharacterMovement()->BrakingDecelerationWalking = _brakingDecelerationWalking;
}

void AObstacleIceBlock::Multicast_ReplicateOnWalkOut_Implementation(APlayerCharacter* _player, float _groundFriction, float _brakingDecelerationWalking)
{
	UKismetSystemLibrary::PrintString(_player, "Pas Glisse");

	_player->GetCharacterMovement()->GroundFriction = groundFrictionNormal;
	_player->GetCharacterMovement()->BrakingDecelerationWalking = brakingDecelerationWalkingNormal;
}


